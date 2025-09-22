import http.server
import socketserver
import json

class Handler(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/':
            self.send_response(200)
            self.send_header('Content-type', 'text/plain')
            self.end_headers()
            self.wfile.write(b'OK')
        else:
            self.send_response(404)
            self.end_headers()

    def do_POST(self):
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        response = {'response': 'Mock Ollama response', 'done': True}
        self.wfile.write(json.dumps(response).encode())

with socketserver.TCPServer(('0.0.0.0', 11434), Handler) as httpd:
    print("serving at port", 11434)
    httpd.serve_forever()