import subprocess
import os
import sys

class XhellWrapper:
    """Wrapper class to interact with the Xhell C program"""
    
    def __init__(self, xhell_path="./xhell/xhell", workspace_dir="./demo_workspace"):
        self.xhell_path = xhell_path
        self.workspace_dir = os.path.abspath(workspace_dir)
        self.history = []
        self.log_file = os.path.join(self.workspace_dir, ".xhell_log")
        
        # Ensure workspace exists
        if not os.path.exists(self.workspace_dir):
            os.makedirs(self.workspace_dir)
            
    def execute_command(self, command):
        """Execute a single command in xhell"""
        try:
            # Ensure we use absolute path for the executable
            abs_xhell_path = os.path.abspath(self.xhell_path)
            
            # Use echo to pipe command to xhell
            process = subprocess.Popen(
                f'echo "{command}" | {abs_xhell_path}',
                shell=True,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                cwd=self.workspace_dir
            )
            
            stdout, stderr = process.communicate(timeout=5)
            returncode = process.returncode
            
            # Filter out proxychains noise
            if stdout:
                stdout = '\n'.join([line for line in stdout.split('\n') if '[proxychains]' not in line])
            if stderr:
                stderr = '\n'.join([line for line in stderr.split('\n') if '[proxychains]' not in line])
            
            # Add to history
            self.history.append({
                'command': command,
                'stdout': stdout,
                'stderr': stderr,
                'returncode': returncode
            })
            
            return {
                'stdout': stdout,
                'stderr': stderr,
                'returncode': returncode,
                'success': returncode == 0
            }
            
        except subprocess.TimeoutExpired:
            process.kill()
            return {
                'stdout': '',
                'stderr': 'Command timed out',
                'returncode': -1,
                'success': False
            }
        except Exception as e:
            return {
                'stdout': '',
                'stderr': str(e),
                'returncode': -1,
                'success': False
            }
    
    def execute_commands_batch(self, commands):
        """Execute multiple commands"""
        results = []
        for cmd in commands:
            result = self.execute_command(cmd)
            results.append(result)
        return results
    
    def get_history(self):
        """Get command history"""
        return self.history
    
    def get_logs(self):
        """Read xhell log file"""
        try:
            if os.path.exists(self.log_file):
                with open(self.log_file, 'r') as f:
                    return f.read()
            return "No logs available"
        except Exception as e:
            return f"Error reading logs: {str(e)}"
    
    def clear_logs(self):
        """Clear log file"""
        try:
            if os.path.exists(self.log_file):
                os.remove(self.log_file)
            return True
        except:
            return False
