FROM ubuntu:22.04

LABEL maintainer="bniladridas"
LABEL description="Llamaware Agent - Professional AI Agent"
LABEL version="0.1"

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    nlohmann-json3-dev \
    libcurl4-openssl-dev \
    libssl-dev \
    zlib1g-dev \
    curl \
    git \
    && rm -rf /var/lib/apt/lists/*

# Build CPR from source since it's not in Ubuntu repos
RUN git clone https://github.com/libcpr/cpr.git /tmp/cpr && \
    cd /tmp/cpr && \
    cmake -S . -B build -DCPR_USE_SYSTEM_CURL=ON -DCPR_ENABLE_SSL=ON -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build --parallel && \
    cmake --install build && \
    rm -rf /tmp/cpr

# Create app directory
WORKDIR /app

# Copy source code
COPY . .

# Build the application
RUN mkdir build && cd build && \
    cmake .. && \
    make && \
    cp bin/llamaware-agent /usr/local/bin/

# Create data directory
RUN mkdir -p /app/data

# Create non-root user
RUN useradd -m -s /bin/bash llamaware
USER llamaware

# Set working directory
WORKDIR /home/llamaware

# Copy configuration template
COPY --chown=llamaware:llamaware .env.example .env

# Expose any ports if needed (none for this CLI app)
# EXPOSE 8080

# Health check
HEALTHCHECK --interval=30s --timeout=10s --start-period=5s --retries=3 \
    CMD llamaware-agent --version || exit 1

# Default command
CMD ["llamaware-agent"]