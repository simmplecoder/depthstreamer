// include the librealsense C++ header file
#include <librealsense/rs.hpp>

// include OpenCV header file
#include <opencv2/opencv.hpp>
#include <cstdint>
#include <fstream>
#include <thread>
#include <utility>
#include <memory>

using namespace std;
using namespace cv;

class depthmap_streamer
{
    rs::device* device;
    std::atomic_flag flag;
public:
    depthmap_streamer(rs::context& context):
            device(context.get_device(0))
    {
        flag.test_and_set();
        namedWindow("Display Image", WINDOW_AUTOSIZE );

        device->enable_stream(rs::stream::depth, 480, 360, rs::format::z16, 60);
        device->start();

        //let camera stabilize
        for(int i = 0; i < 30; i++)
            device->wait_for_frames();
    }

    void stop()
    {
        flag.clear();
    }

    void run()
    {
        while (flag.test_and_set())
        {
            display_frame();
        }
    }
private:
    void display_frame()
    {
        device->wait_for_frames();

        auto frame_ptr = (void*)device->get_frame_data(rs::stream::depth);
        Mat depth(Size(480, 360), CV_16U, frame_ptr, Mat::AUTO_STEP);
        Mat depth8u;
        depth.convertTo(depth8u, CV_8U);

        imshow("Display Image", depth8u);
    }
};

int main()
{
    // Create a context object. This object owns the handles to all connected realsense devices
    rs::context ctx;
    depthmap_streamer streamer(ctx);

    auto stream = [&streamer]()
    {
        streamer.run();
    };

    std::thread streaming_thread{stream};

    waitKey(0);
    streamer.stop();
    streaming_thread.join();

    return 0;
}
