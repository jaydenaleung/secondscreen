from flask import Flask, render_template, request, jsonify, send_from_directory
import os
import time
import hashlib
import re
from werkzeug.utils import safe_join
import subprocess
import sys
try:
    import applescript
except ImportError:
    applescript = None


app = Flask(__name__)
UPLOAD_FOLDER = 'uploads' 
MAX_FILE_SIZE = 1024 * 1024 
MAX_FOLDER_SIZE = 1024 * 1024 * 1024  

app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER
os.makedirs(UPLOAD_FOLDER, exist_ok=True) 

def manage_storage():
    """Deletes the oldest file in the upload folder if the total size exceeds the limit."""
    folder_size = 0
    files = []
    for filename in os.listdir(app.config['UPLOAD_FOLDER']):
        filepath = os.path.join(app.config['UPLOAD_FOLDER'], filename)
        if os.path.isfile(filepath):
            filesize = os.path.getsize(filepath)
            folder_size += filesize
            files.append((filepath, filesize))

    if folder_size > MAX_FOLDER_SIZE:
        oldest_file = min(files, key=lambda x: os.path.getmtime(x[0])) 
        os.remove(oldest_file[0])
        print(f"Deleted oldest file: {oldest_file[0]} to manage storage.")

def is_regular_text(text):
    if not isinstance(text, str):
        return False
    return bool(re.match('[^\w]', text))

# --- Apple Music Control Functions ---
def is_apple_music_running():
    script = 'tell application "System Events" to (name of processes) contains "Music"'
    if applescript:
        return applescript.AppleScript(script).run().out == 'true'
    else:
        # Fallback to osascript
        result = subprocess.run(['osascript', '-e', script], capture_output=True, text=True)
        return 'true' in result.stdout

def apple_music_command(cmd):
    if cmd not in ("playpause", "next track", "previous track"): return False
    script = f'tell application "Music" to {cmd}'
    if applescript:
        applescript.AppleScript(script).run()
    else:
        subprocess.run(['osascript', '-e', script])
    return True

def get_apple_music_info():
    script = '''
    tell application "System Events" to (name of processes) contains "Music"
    if result then
        tell application "Music"
            set songTitle to the name of the current track
            set songArtist to the artist of the current track
            set playerState to the player state as string
            return songTitle & "\n" & songArtist & "\n" & playerState
        end tell
    else
        return "\n\nstopped"
    end if
    '''
    if applescript:
        out = applescript.AppleScript(script).run().out
    else:
        out = subprocess.run(['osascript', '-e', script], capture_output=True, text=True).stdout
    parts = out.strip().split('\n')
    if len(parts) < 3:
        return {'title': '', 'artist': '', 'state': 'stopped'}
    return {'title': parts[0], 'artist': parts[1], 'state': parts[2]}

@app.route('/api/apple_music/status', methods=['GET'])
def apple_music_status():
    return jsonify({'running': is_apple_music_running()})

@app.route('/api/apple_music/control', methods=['POST'])
def apple_music_control():
    data = request.get_json(force=True)
    cmd = data.get('command')
    if not is_apple_music_running():
        return jsonify({'success': False, 'error': 'Apple Music is not running'}), 400
    if cmd not in ("playpause", "next track", "previous track"):
        return jsonify({'success': False, 'error': 'Invalid command'}), 400
    apple_music_command(cmd)
    return jsonify({'success': True})

@app.route('/api/apple_music/info', methods=['GET'])
def apple_music_info():
    return jsonify(get_apple_music_info())

@app.route('/api/pico/button', methods=['POST'])
def pico_button():
    data = request.get_json(force=True)
    btn = data.get('button')
    if btn not in ("playpause", "next", "previous"): return jsonify({'success': False}), 400
    # Forward to Apple Music
    if btn == "playpause":
        apple_music_command("playpause")
    elif btn == "next":
        apple_music_command("next track")
    elif btn == "previous":
        apple_music_command("previous track")
    return jsonify({'success': True})

@app.route('/api/upload', methods=['POST'])
def upload_file():
    manage_storage() 

    if 'audio' not in request.files:
        return jsonify({'message': 'No audio file provided'}), 400
    
    file = request.files['audio']

    if is_regular_text(file.filename):
        return jsonify({'message': 'Invalid file name'}), 400
    
    if file.filename == '':
        return jsonify({'message': 'No selected file'}), 400

    if len(file.read()) > MAX_FILE_SIZE: 
        return jsonify({'message': 'File size exceeds 1MB limit'}), 413 
    print(file.filename)
    file.seek(0)
    file_content = file.read()
    file.seek(0)
    file_hash = hashlib.sha256(file_content).hexdigest()

    for existing_filename in os.listdir(app.config['UPLOAD_FOLDER']):
        existing_filepath = safe_join(app.config['UPLOAD_FOLDER'], existing_filename)
        if os.path.isfile(existing_filepath):
            with open(existing_filepath, 'rb') as f:
                existing_content = f.read()
                existing_hash = hashlib.sha256(existing_content).hexdigest()
                if file_hash == existing_hash:
                    return jsonify({'message': 'An identical recording already exists.'}), 409 
    
    filename = safe_join(app.config['UPLOAD_FOLDER'], str(int(time.time()))+"-"+file.filename)

    try:
        file.save(filename)
    except Exception as e:
        return jsonify({'message': f"Error saving file: {str(e)}"}), 500
    return jsonify({'message': 'File uploaded successfully'}), 201

@app.route('/api/recordings')
def list_recordings():
    """Returns a list of available recordings."""
    try:
        recordings = [
            f for f in os.listdir(app.config['UPLOAD_FOLDER'])
            if os.path.isfile(safe_join(app.config['UPLOAD_FOLDER'], f)) 
        ]
        return jsonify(recordings)

    except FileNotFoundError: 
        return jsonify({'message': "Uploads directory not found"}), 500 
    except Exception as e:
        return jsonify({'message': f"Error listing recordings: {str(e)}"}), 500

@app.route("/recordings")
def rec():
    return render_template('recordings.html')


@app.route('/uploads/<filename>')
def uploaded_file(filename):
    return send_from_directory("",safe_join(app.config['UPLOAD_FOLDER'], filename))

import requests

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/api/weather')
def get_weather():
    # Use a free weather API that requires no auth, e.g. wttr.in
    try:
        response = requests.get('https://wttr.in/San+Francisco?format=j1')
        data = response.json()
        # Extract relevant data
        current_condition = data['current_condition'][0]
        temp_f = current_condition['temp_F']
        humidity = current_condition['humidity']
        wind_mph = current_condition['windspeedMiles']
        weather_desc = current_condition['weatherDesc'][0]['value']
        return {
            'temp_f': temp_f,
            'humidity': humidity,
            'wind_mph': wind_mph,
            'description': weather_desc
        }
    except Exception as e:
        return {'error': str(e)}


if __name__ == '__main__':
    app.run(host='0.0.0.0',port=80,debug=True)
