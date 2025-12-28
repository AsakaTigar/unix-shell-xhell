import os
import sys
import re
from flask import Flask, render_template, request, jsonify
from xhell_wrapper import XhellWrapper

app = Flask(__name__)

# Initialize Xhell Wrapper
workspace_dir = "./demo_workspace"
if not os.path.exists(workspace_dir):
    os.makedirs(workspace_dir)
xhell = XhellWrapper(workspace_dir=workspace_dir)

def strip_ansi(text):
    """Remove ANSI escape sequences from text"""
    ansi_escape = re.compile(r'\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])')
    return ansi_escape.sub('', text)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/cli')
def cli():
    return render_template('cli.html')

@app.route('/execute', methods=['POST'])
def execute_command():
    data = request.json
    cmd = data.get('command')
    
    if not cmd:
        return jsonify({'error': 'No command provided'}), 400
        
    result = xhell.execute_command(cmd)
    
    response = {
        'command': cmd,
        'stdout': strip_ansi(result['stdout']),
        'stderr': strip_ansi(result['stderr']),
        'cwd': os.getcwd() # Or xhell's internal cwd if it tracks it
    }
    return jsonify(response)

@app.route('/files', methods=['GET'])
def list_files():
    try:
        files = sorted([f for f in os.listdir(workspace_dir) if not f.startswith('.')])
        return jsonify({'files': files})
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/read_file', methods=['GET'])
def read_file():
    filename = request.args.get('filename')
    if not filename:
        return jsonify({'error': 'No filename provided'}), 400
        
    filepath = os.path.join(workspace_dir, filename)
    
    # Security check to prevent directory traversal
    if not os.path.abspath(filepath).startswith(os.path.abspath(workspace_dir)):
         return jsonify({'error': 'Access denied'}), 403

    try:
        with open(filepath, 'r') as f:
            content = f.read(2000) # Limit size
            return jsonify({'content': content})
    except Exception as e:
        return jsonify({'error': str(e)}), 500

if __name__ == '__main__':
    app.run(debug=True, port=8505)
