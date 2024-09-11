#!/bin/bash

# Function to clean up processes and temporary files
cleanup() {
    echo "Cleaning up..."
    kill $AUDIO_PID 2>/dev/null
    kill $VIDEO_PID 2>/dev/null
    rm -f "$AUDIO_FILE"
    exit
}

# Set up trap to call cleanup function on script exit
trap cleanup EXIT INT TERM

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <video_file>"
    exit 1
fi

VIDEO_FILE="$1"

# Extract audio to a temporary file
AUDIO_FILE=$(mktemp).wav
ffmpeg -i "$VIDEO_FILE" -vn -acodec pcm_s16le -ar 44100 -ac 2 "$AUDIO_FILE"

# Start audio playback in the background
afplay "$AUDIO_FILE" &
AUDIO_PID=$!

# Run the video ASCII converter in the background
./video_to_ascii "$VIDEO_FILE" &
VIDEO_PID=$!

# Wait for the video process to finish
wait $VIDEO_PID

# The cleanup function will be called automatically when the script exits
