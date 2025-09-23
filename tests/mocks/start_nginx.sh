#!/bin/sh
set -e

# Create nginx config directory
mkdir -p /tmp/nginx

# Create nginx config
cat > /tmp/nginx/nginx.conf << 'EOF'
error_log /dev/stderr warn;
pid /tmp/nginx/nginx.pid;

events {
    worker_connections 1024;
}

http {
    server {
        listen 80;
        access_log /dev/stdout;
        error_log /dev/stderr;
        
        location = /health {
            add_header Content-Type text/plain;
            return 200 "healthy\n";
        }
    }
}
EOF

# Start nginx
exec nginx -c /tmp/nginx/nginx.conf -g 'daemon off;'
