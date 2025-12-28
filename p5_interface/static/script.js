function runCmd(command) {
    fetch('/execute', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ command: command })
    })
        .then(response => response.json())
        .then(data => {
            addToTerminal(data);
            refreshFiles(); // Auto refresh files on command
        })
        .catch(error => console.error('Error:', error));
}

function handleManualCmd() {
    const input = document.getElementById('cmd-input');
    const cmd = input.value.trim();
    if (cmd) {
        runCmd(cmd);
        input.value = '';
    }
}

// Allow Enter key to submit
document.getElementById('cmd-input').addEventListener('keypress', function (e) {
    if (e.key === 'Enter') {
        handleManualCmd();
    }
});

function addToTerminal(data) {
    const term = document.getElementById('terminal-output');

    // Remove welcome message if present
    const welcome = term.querySelector('.welcome-msg');
    if (welcome) welcome.remove();

    const entry = document.createElement('div');
    entry.className = 'log-entry';

    let html = `<div class="cmd-text">> ${data.command}</div>`;

    if (data.stdout) {
        html += `<div class="output-text">${data.stdout}</div>`;
    }

    if (data.stderr) {
        html += `<div class="error-text">ERROR: ${data.stderr}</div>`;
    }

    entry.innerHTML = html;
    term.insertBefore(entry, term.firstChild); // Prepend to show newest first? Or append? Usually terminals append.
    // Let's prepend to match the previous Streamlit style (Newest at top) or Append (Classic terminal).
    // Streamlit app did "Newest First". A real terminal is "Newest Last". 
    // Let's go with "Newest First" as it's easier to see without scrolling in a HUD UI.
    // Actually, for a terminal feel, scrolling down is better. Let's append.

    // Wait, let's stick to 'prepend' for now as the CSS might not handle auto-scroll well without more logic.
    // Actually, let's PREPEND so the user sees the result immediately at the top.

}

function refreshFiles() {
    fetch('/files')
        .then(response => response.json())
        .then(data => {
            const list = document.getElementById('file-list');
            list.innerHTML = '';
            if (data.files) {
                data.files.forEach(file => {
                    const item = document.createElement('div');
                    item.className = 'file-item';
                    item.textContent = file;
                    item.onclick = () => viewFile(file);
                    list.appendChild(item);
                });
            }
        });
}

function viewFile(filename) {
    fetch(`/read_file?filename=${encodeURIComponent(filename)}`)
        .then(response => response.json())
        .then(data => {
            const viewer = document.getElementById('file-content');
            if (data.content) {
                viewer.textContent = data.content;
            } else if (data.error) {
                viewer.textContent = `Error: ${data.error}`;
            }
        });
}

function resetWorkspace() {
    if (confirm("Are you sure you want to delete all files in the workspace?")) {
        // We'd need a backend endpoint for this or just run 'rm -rf' via execute
        runCmd("rm -rf demo_workspace/*");
        setTimeout(refreshFiles, 500);
    }
}

// Initial load
refreshFiles();
