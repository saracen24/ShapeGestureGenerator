#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

static void Usage() {
  cerr << "Usage: " << endl
       << "./" << string(PROJECT_NAME)
       << " <ellipse|rectangle|triangle> <reduce_iterations> <abs_dispersion> "
          "<closed_contour> <delay>"
       << endl;
}

void reduce(vector<Point>& v, size_t every, size_t from) {
  size_t step;
  for (step = 1; from < v.size(); ++step, from += every) {
    for (size_t i = 1; i < every && from + i < v.size(); ++i)
      v[from + i - step] = v[from + i];
  }
  v.resize(v.size() - (step - 1));
}

int main(int argc, char* argv[]) {
  if (argc != 6) {
    Usage();
    return EXIT_FAILURE;
  }

  const string shape = argv[1];
  const size_t reduce_iterations = stoi(argv[2]);
  const int abs_dispersion = stoi(argv[3]);
  const bool closed_contour = stoi(argv[4]);
  const int delay = stoi(argv[5]);

  // Check arguments.
  static const array<const string, 3> kShapes{"ellipse", "rectangle",
                                              "triangle"};
  if (find(kShapes.begin(), kShapes.end(), shape) == kShapes.end()) {
    Usage();
    return EXIT_FAILURE;
  }

  const String windowName("Canvas");
  const Size windowSize(512, 512);
  namedWindow(windowName,
              WINDOW_NORMAL | WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED);
  resizeWindow(windowName, windowSize);

  RNG rng(getTickCount());
  const Point2f center(windowSize.width * 0.5f, windowSize.height * 0.5f);
  bool clockwise = false;

  auto drawRandomEllipse = [&rng, &center](Mat& canvas) -> void {
    RotatedRect rrect(center,
                      Size2f(canvas.cols / rng.uniform(1.3f, 3.f),
                             canvas.rows / rng.uniform(1.3f, 3.f)),
                      rng.uniform(0.f, 360.f));

    ellipse(canvas, rrect, CV_RGB(0, 0, 255), 3, LINE_AA);
  };

  auto drawRandomRectangle = [&rng, &center](Mat& canvas) -> void {
    RotatedRect rrect(center,
                      Size2f(canvas.cols / rng.uniform(1.3f, 3.f),
                             canvas.rows / rng.uniform(1.3f, 3.f)),
                      rng.uniform(0.f, 360.f));

    Point2f vertices[4];
    rrect.points(vertices);
    for (size_t i = 0; i < 4; i++)
      line(canvas, vertices[i], vertices[(i + 1) % 4], CV_RGB(0, 0, 255), 3,
           LINE_AA);
  };

  auto drawRandomTriangle = [&rng, &center](Mat& canvas) -> void {
    RotatedRect rrect(center,
                      Size2f(canvas.cols / rng.uniform(1.3f, 3.f),
                             canvas.rows / rng.uniform(1.3f, 3.f)),
                      rng.uniform(0.f, 360.f));

    Point2f vertices[4];
    rrect.points(vertices);
    Point2f sideCenter = (vertices[2] + vertices[3]) * 0.5f;
    vector<Point2f> corners;
    for (size_t i = 0; i < 4; i++) {
      if (i == 3)
        continue;
      else if (i == 2)
        corners.push_back(sideCenter);
      else
        corners.push_back(vertices[i]);
    }

    for (size_t i = 0; i < 3; i++)
      line(canvas, corners[i], corners[(i + 1) % 3], CV_RGB(0, 0, 255), 3,
           LINE_AA);
  };

  Mat canvas;
  for (size_t iteration = 0; true; iteration++) {
    canvas = Mat::zeros(windowSize, CV_8UC3);

    // Draw random shape.
    if (shape == "ellipse")
      drawRandomEllipse(canvas);
    else if (shape == "rectangle")
      drawRandomRectangle(canvas);
    else if (shape == "triangle")
      drawRandomTriangle(canvas);

    // Find contour.
    Mat gray;
    cvtColor(canvas, gray, COLOR_BGR2GRAY);
    vector<vector<Point>> contours;
    findContours(gray, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    vector<Point>& contour = contours.at(0);

    // Reduce contour points by "reduce_iterations" times.
    for (size_t i = 0; i < reduce_iterations; i++) reduce(contour, 2, 0);

    // Add "abs_dispersion" noise to points.
    for (Point& pt : contour) {
      pt.x += rng.uniform(-abs_dispersion, abs_dispersion);
      pt.y += rng.uniform(-abs_dispersion, abs_dispersion);
    }

    // Change direction.
    if (clockwise) reverse(contour.begin(), contour.end());
    clockwise = !clockwise;

    // Start point.
    drawMarker(canvas, contour[0], CV_RGB(0, 255, 0), MARKER_TILTED_CROSS, 20,
               2, LINE_AA);

    // Draw direction contour. Out to console and file.
    cout << ">> Iteration: " << iteration << "; Points: " << contour.size()
         << endl;
    ofstream datfile(shape + "_" + to_string(iteration) + ".dat",
                     ios::out | ios::trunc);

    for (size_t i = 0; i < contour.size() - !closed_contour; i++) {
      arrowedLine(canvas, contour[i], contour[(i + 1) % contour.size()],
                  CV_RGB(255, 0, 0), 2, LINE_AA, 0, 0.3);

      Point absoluteOffset = contour[(i + 1) % contour.size()] - contour[i];
      Point2f relativeOffset =
          Point2f(absoluteOffset.x / static_cast<float>(windowSize.width),
                  absoluteOffset.y / static_cast<float>(windowSize.height));

      cout << std::internal << setfill(' ') << setw(2) << absoluteOffset;
      cout << std::internal << setfill(' ') << setw(4) << relativeOffset
           << endl;

      datfile << relativeOffset.x << "," << relativeOffset.y << endl;
    }

    datfile.close();
    cout << endl;

    imshow(windowName, canvas);
    const char key = static_cast<const char>(waitKey(delay) & 0xFF);
    if (key == 'q') break;
  }

  return 0;
}
