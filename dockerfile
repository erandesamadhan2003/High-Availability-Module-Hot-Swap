FROM ubuntu:20.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies (use ninja + ccache for faster builds)
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential cmake git ninja-build ccache ca-certificates \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy source
COPY . /app

# Configure and build only the demo target with Ninja (faster) and parallel jobs.
# Building a single target reduces total work and dramatically speeds up Docker builds.
RUN mkdir -p build && cd build && \
    cmake -G Ninja -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . --target advanced_demo -- -j$(nproc)

FROM ubuntu:20.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive

# Minimal runtime deps (keep image small)
RUN apt-get update && apt-get install -y --no-install-recommends ca-certificates \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy only the built artifacts from the builder stage
COPY --from=builder /app/build /app/build

# Ensure the dynamic loader can find the .so files produced by the build
ENV LD_LIBRARY_PATH=/app/build

CMD ["/app/build/advanced_demo"]