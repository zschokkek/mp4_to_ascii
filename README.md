# Terminal Video Player with Audio

A terminal-based video player that converts video to ASCII art and plays synchronized audio. This project combines video-to-ASCII conversion with audio playback for a complete terminal multimedia experience.

## Overview

The project consists of two main components:
- A C++ program that converts video frames to ASCII art and displays them in the terminal
- A bash script that handles audio extraction and synchronization

## Requirements

- C++11 compatible compiler
- OpenCV library
- ffmpeg for audio extraction
- macOS (for afplay) or Linux (requires modification for alternative audio player)

## Installation

1. Install dependencies:
```bash
# Ubuntu/Debian
sudo apt-get install libopencv-dev ffmpeg

# macOS
brew install opencv ffmpeg


2. Compile the video converter:
```bash
g++ -o video_to_ascii main.cpp `pkg-config --cflags --libs opencv4`
```

3. Make the run script executable:
```bash
chmod +x run_video_with_audio.sh
```

## Usage

Run a video file with audio:
```bash
./run_video_with_audio.sh <video_file>
```

Run video only:
```bash
./video_to_ascii <video_file>
```

## Implementation Details

### Video Converter (main.cpp)
- Reads video frames using OpenCV
- Converts frames to grayscale
- Scales output to terminal dimensions
- Maps pixel intensities to ASCII characters
- Optimizes display refresh for minimal flicker

### Audio Synchronization (run_video_with_audio.sh)
- Extracts audio track from video file
- Manages parallel processes for video and audio playback
- Handles cleanup of temporary files
- Manages process termination

## Configuration

The video converter can be configured by modifying these constants:
```cpp
const std::string ASCII_CHARS = " .:-=+*#%@";  // Character set for conversion
config.max_fps = 30;                           // Frame rate limit
config.invert = false;                         // Invert output
```

