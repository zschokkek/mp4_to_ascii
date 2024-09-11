#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/ioctl.h>
#include <opencv2/opencv.hpp>

const std::string ASCII_CHARS = " .:-=+*#%@";

struct VideoConfig {
    std::string video_path;
    int max_fps;
    bool invert;
};

struct TerminalSize {
    int rows;
    int cols;
};

TerminalSize get_terminal_size() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return {w.ws_row, w.ws_col};
}

char get_ascii_char(unsigned char pixel_value, bool invert) {
    int index = pixel_value * (ASCII_CHARS.length() - 1) / 255;
    if (invert) {
        index = ASCII_CHARS.length() - 1 - index;
    }
    return ASCII_CHARS[index];
}

void frame_to_ascii(const cv::Mat& frame, std::vector<std::string>& ascii_frame, const VideoConfig& config, const TerminalSize& term_size) {
    int scaled_width = term_size.cols;
    int scaled_height = term_size.rows - 1;  // Leave one row for prompt

    ascii_frame.resize(scaled_height);

    for (int y = 0; y < scaled_height; y++) {
        ascii_frame[y].resize(scaled_width);
        for (int x = 0; x < scaled_width; x++) {
            int pixel_x = x * frame.cols / scaled_width;
            int pixel_y = y * frame.rows / scaled_height;
            unsigned char pixel = frame.at<uchar>(pixel_y, pixel_x);
            ascii_frame[y][x] = get_ascii_char(pixel, config.invert);
        }
    }
}

void process_video(const VideoConfig& config) {
    cv::VideoCapture capture(config.video_path);
    if (!capture.isOpened()) {
        std::cerr << "Could not open video file" << std::endl;
        return;
    }

    int frame_count = capture.get(cv::CAP_PROP_FRAME_COUNT);
    double fps = capture.get(cv::CAP_PROP_FPS);
    int frame_interval = fps / config.max_fps;

    std::vector<std::string> ascii_frame;
    TerminalSize term_size = get_terminal_size();

    for (int i = 0; i < frame_count; i++) {
        cv::Mat frame;
        capture >> frame;
        if (frame.empty()) break;

        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);

        if (i % frame_interval == 0) {
            frame_to_ascii(frame, ascii_frame, config, term_size);

            // Instead of clearing, let's prepare the full output
            std::string output;
            for (const auto& line : ascii_frame) {
                output += line + "\n";
            }

            // Write the entire frame at once to reduce flicker
            std::cout << "\033[H" << output << std::flush;

            usleep(642000 / config.max_fps);  // Adjusted sleep time
        }
    }
}
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <video_path>" << std::endl;
        return 1;
    }

    VideoConfig config = {
        argv[1],
        30,
        false
    };

    process_video(config);

    return 0;
}