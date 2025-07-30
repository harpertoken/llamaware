FROM ubuntu:22.04

LABEL maintainer="bniladridas"
LABEL description="Llamaware Agent - Professional AI Agent"
LABEL version="2.0.0"

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libcpr-dev \
    nlohmann-json3-dev \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Create app directory
WORKDIR /app

# Copy source code
COPY . .

# Build the application
RUN mkdir build && cd build && \
    cmake .. && \
    make && \
    cp package/cli/bin/llamaware-agent /usr/local/bin/

# Create data directory
RUN mkdir -p /app/data

# Create non-root user
RUN useradd -m -s /bin/bash llamaware
USER llamaware

# Set working directory
WORKDIR /home/llamaware

# Copy configuration template
COPY --chown=llamaware:llamaware package/cli/.env.example .env

# Expose any ports if needed (none for this CLI app)
# EXPOSE 8080

# Health check
HEALTHCHECK --interval=30s --timeout=10s --start-period=5s --retries=3 \
    CMD llamaware-agent --version || exit 1

# Default command
CMD ["llamaware-agent"]