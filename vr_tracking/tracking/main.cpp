#include <iostream>
#include <vector>
#include <string>
#include <openvr.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include "SDL2/SDL_ttf.h"
#include "utils.h"
#include <zmq.hpp>
//#include <paho-mqtt3a/client.h>
#include "mqtt/client.h"
//#include "mqtt/async_client.h"
#include <unistd.h>


// ----------------
// OpenVR variables
// ----------------

vr::IVRSystem *vr_context;
vr::TrackedDevicePose_t tracked_device_pose[vr::k_unMaxTrackedDeviceCount];

// ----------------
// app variables
// ----------------

bool app_end = false;
string driver_name, driver_serial;
string tracked_device_type[vr::k_unMaxTrackedDeviceCount];

// ----------------
// app functions
// ----------------

int init_SDL();

int init_OpenVR();

void process_vr_event(const vr::VREvent_t &event);

void exit();

vr::HmdQuaternion_t GetRotation(vr::HmdMatrix34_t matrix) {
    vr::HmdQuaternion_t q;

    q.w = sqrt(fmax(0, 1 + matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2])) / 2;
    q.x = sqrt(fmax(0, 1 + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2])) / 2;
    q.y = sqrt(fmax(0, 1 - matrix.m[0][0] + matrix.m[1][1] - matrix.m[2][2])) / 2;
    q.z = sqrt(fmax(0, 1 - matrix.m[0][0] - matrix.m[1][1] + matrix.m[2][2])) / 2;
    q.x = copysign(q.x, matrix.m[2][1] - matrix.m[1][2]);
    q.y = copysign(q.y, matrix.m[0][2] - matrix.m[2][0]);
    q.z = copysign(q.z, matrix.m[1][0] - matrix.m[0][1]);
    return q;
}

/**
 * A callback class for use with the main MQTT client.
 */
class callback : public virtual mqtt::callback
{
public:
    void connection_lost(const string& cause) override {
        cout << "\nConnection lost" << endl;
        if (!cause.empty())
            cout << "\tcause: " << cause << endl;
    }

    void delivery_complete(mqtt::delivery_token_ptr tok) override {
//        cout << "\tDelivery complete for token: "
//             << (tok ? tok->get_message_id() : -1) << endl;
    }
};

/**
 * A base action listener.
 */
class action_listener : public virtual mqtt::iaction_listener
{
protected:
    void on_failure(const mqtt::token& tok) override {
        cout << "\tListener failure for token: "
             << tok.get_message_id() << endl;
    }

    void on_success(const mqtt::token& tok) override {
//        cout << "\tListener success for token: "
//             << tok.get_message_id() << endl;
    }
};

int main(int argv, char **args) {

    if (init_SDL() != 0) return -1;
    if (init_OpenVR() != 0) return -1;
    bool MQTT = false;

    // MQTT
    const string PERSIST_DIR			{ "./persist" };
    string address = "tcp://0.0.0.0:1883";
    string clientID = "8";
    const string TOPIC { "VR_CONTEXT" };
    const int  QOS = 0;
//    const char* LWT_PAYLOAD = "Last will and testament.";

    cout << "Initializing for server '" << address << "'..." << endl;
    mqtt::async_client client(address, clientID, PERSIST_DIR);
    cout << "test" << endl;
    callback cb;
    client.set_callback(cb);

    auto connOpts = mqtt::connect_options_builder()
            .clean_session(true)
            .finalize();
//    .will(mqtt::message(TOPIC, LWT_PAYLOAD, QOS))


    mqtt::token_ptr conntok;

    if (MQTT) {
        cout << "  ...OK" << endl;

        cout << "\nConnecting..." << endl;
        conntok = client.connect(connOpts);
        cout << "Waiting for the connection..." << endl;
        conntok->wait();
        cout << "  ...OK" << endl;
    }

    // ZMQ
    zmq::context_t ctx;
    zmq::socket_t sock(ctx, zmq::socket_type::pair);
    if (not MQTT) {
        sock.bind("tcp://127.0.0.1:5555");
    }

    while (!app_end) {
        SDL_RenderClear(renderer); // Clear screen in order to render new stuff

        print_text(("Driver name: " + driver_name).c_str(), black, 10, 10);
        print_text(("Driver serial ID: " + driver_serial).c_str(), black, 10, 40);

        if (vr_context != NULL) {
            // Process SteamVR events
            vr::VREvent_t vr_event;
            while (vr_context->PollNextEvent(&vr_event, sizeof(vr_event)))
                process_vr_event(vr_event);

            // Obtain tracking device poses
            vr_context->GetDeviceToAbsoluteTrackingPose(vr::ETrackingUniverseOrigin::TrackingUniverseStanding, 0,
                                                        tracked_device_pose, vr::k_unMaxTrackedDeviceCount);

            int actual_y = 110, tracked_device_count = 0;
//            cout << vr::k_unMaxTrackedDeviceCount << endl;
            for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; nDevice++) {
                if ((tracked_device_pose[nDevice].bDeviceIsConnected) && (tracked_device_pose[nDevice].bPoseIsValid)) {
                    SDL_Color color = green;

                    // Check whether the tracked device is a controller. If so, set text color based on the trigger button state
                    vr::VRControllerState_t controller_state;
                    if (vr_context->GetControllerState(nDevice, &controller_state, sizeof(controller_state)))
                        ((vr::ButtonMaskFromId(vr::EVRButtonId::k_EButton_Axis1) & controller_state.ulButtonPressed) ==
                         0) ? color = green : color = blue;

                    print_text(("Tracked device #" + ftos((float) nDevice, 0) + " (" + tracked_device_type[nDevice] +
                                ")").c_str(), color, 10, actual_y);

                    // We take just the translation part of the matrix (actual position of tracked device, not orientation)
                    float v[3] = {tracked_device_pose[nDevice].mDeviceToAbsoluteTracking.m[0][3],
                                  tracked_device_pose[nDevice].mDeviceToAbsoluteTracking.m[1][3],
                                  tracked_device_pose[nDevice].mDeviceToAbsoluteTracking.m[2][3]};


                    std::string pos3D =
                            std::to_string(tracked_device_pose[nDevice].mDeviceToAbsoluteTracking.m[0][3]) + "," +
                            std::to_string(tracked_device_pose[nDevice].mDeviceToAbsoluteTracking.m[1][3]) + "," +
                            std::to_string(tracked_device_pose[nDevice].mDeviceToAbsoluteTracking.m[2][3]);

                    std::string device = "UNK,";
                    if (nDevice == 0) {
                        vr::HmdQuaternion_t quaternion = GetRotation(
                                tracked_device_pose[nDevice].mDeviceToAbsoluteTracking);
                        std::string quat = std::to_string(quaternion.w) + ',' + std::to_string(quaternion.x) + ',' +
                                           std::to_string(quaternion.y) + ',' + std::to_string(quaternion.z);
                        device = "HMD,";
                        std::string msg = device + pos3D + ',' + quat;
                        if(MQTT){
                            mqtt::message_ptr pubmsg = mqtt::make_message(TOPIC, msg);
                            pubmsg->set_qos(QOS);
                            client.publish(pubmsg);
                            usleep(10000); //mqtt broker can't handle a faster rate.
                        } else {
                            char data[128];
                            strcpy(data, msg.c_str());
                            sock.send(zmq::str_buffer(data), zmq::send_flags::dontwait);
                        }
                    }

                    print_text(vftos(v, 2).c_str(), color, 50, actual_y + 25);
                    actual_y += 60;

                    tracked_device_count++;
                }
            }

            print_text(("Tracked devices: " + ftos((float) tracked_device_count, 0)).c_str(), black, 10, 70);
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN: {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        cout << "Exiting app..." << endl;

                        app_end = true;
                    }
                }

                    break;
            }
        }

        SDL_RenderPresent(renderer); // Update screen with the new rendered stuff
    }

    exit();

    return 0;
}

int init_SDL() {
    // Init the SDL context
    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        SDL_Quit();

        return -1;
    }

    // Create a window in which render stuff
    window = SDL_CreateWindow("Simple OpenVR example", windowRect.x, windowRect.y, windowRect.w, windowRect.h,
                              0); // Seems that if we don't initialize a windows SDL's event subsystem doesn't work
    if (window == NULL) {
        SDL_Log("Unable to create SDL Window: %s", SDL_GetError());
        SDL_Quit();

        return -1;
    }

    // Create 2D rendering canvas
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_Log("Unable to create SDL Renderer: %s", SDL_GetError());
        SDL_Quit();

        return -1;
    }

    // Set size of renderer to the same size as window
    SDL_RenderSetLogicalSize(renderer, windowRect.w, windowRect.h);

    // Set color of renderer to gray
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

    // Initialize the font library and set the specific font to use when rendering text
    if (TTF_Init() < 0) {
        SDL_Log("Unable to initialize SDL ttf: %s", TTF_GetError());
        SDL_Quit();

        return -1;
    }

    if ((font = TTF_OpenFont("../../thirdparty/resources/fonts/arial.ttf", 20)) == NULL) {
        SDL_Log("Unable to load ttf font: %s", TTF_GetError());
        SDL_Quit();

        return -1;
    }

    SDL_Log("SDL successfully initialized");

    return 0;
}

int init_OpenVR() {
    // Check whether there is an HMD plugged-in and the SteamVR runtime is installed
    if (vr::VR_IsHmdPresent()) {
        cout << "An HMD was successfully found in the system" << endl;

        if (vr::VR_IsRuntimeInstalled()) {
//            const char* runtime_path = vr::VR_GetRuntimePath();
//            cout << "Runtime correctly installed at '" << runtime_path << "'" << endl;
        } else {
            cout << "Runtime was not found, quitting app" << endl;
            return -1;
        }
    } else {
        cout << "No HMD was found in the system, quitting app" << endl;
        return -1;
    }

    // Load the SteamVR Runtime
    vr::HmdError err;
    vr_context = vr::VR_Init(&err, vr::EVRApplicationType::VRApplication_Scene);
    vr_context == NULL ? cout << "Error while initializing SteamVR runtime. Error code is "
                              << vr::VR_GetVRInitErrorAsSymbol(err) << endl : cout
            << "SteamVR runtime successfully initialized" << endl;

    // Obtain some basic information given by the runtime
    int base_stations_count = 0;
    cout << vr::k_unMaxTrackedDeviceCount << endl;

    for (uint32_t td = vr::k_unTrackedDeviceIndex_Hmd; td < vr::k_unMaxTrackedDeviceCount; td++) {

        if (vr_context->IsTrackedDeviceConnected(td)) {
            vr::ETrackedDeviceClass tracked_device_class = vr_context->GetTrackedDeviceClass(td);

            string td_type = GetTrackedDeviceClassString(tracked_device_class);
            tracked_device_type[td] = td_type;

            cout << "Tracking device " << td << " is connected " << endl;
            cout << "  Device type: " << td_type << ". Name: "
                 << GetTrackedDeviceString(vr_context, td, vr::Prop_TrackingSystemName_String) << endl;

            if (tracked_device_class ==
                vr::ETrackedDeviceClass::TrackedDeviceClass_TrackingReference)
                base_stations_count++;

            if (td == vr::k_unTrackedDeviceIndex_Hmd) {
                // Fill variables used for obtaining the device name and serial ID (used later for naming the SDL window)
                driver_name = GetTrackedDeviceString(vr_context, vr::k_unTrackedDeviceIndex_Hmd,
                                                     vr::Prop_TrackingSystemName_String);
                driver_serial = GetTrackedDeviceString(vr_context, vr::k_unTrackedDeviceIndex_Hmd,
                                                       vr::Prop_SerialNumber_String);
            }
        } else
            cout << "Tracking device " << td << " not connected" << endl;
    }

    // Check whether both base stations are found, not mandatory but just in case...
    if (base_stations_count < 2) {
        cout << "There was a problem indentifying the base stations, please check they are powered on" << endl;
        cout << "(or there are none)" << endl;
    }

    return 0;
}

void process_vr_event(const vr::VREvent_t &event) {
    string str_td_class = GetTrackedDeviceClassString(vr_context->GetTrackedDeviceClass(event.trackedDeviceIndex));

    switch (event.eventType) {
        case vr::VREvent_TrackedDeviceActivated: {
            cout << "Device " << event.trackedDeviceIndex << " attached (" << str_td_class << ")" << endl;
            tracked_device_type[event.trackedDeviceIndex] = str_td_class;
        }
            break;
        case vr::VREvent_TrackedDeviceDeactivated: {
            cout << "Device " << event.trackedDeviceIndex << " detached (" << str_td_class << ")" << endl;
            tracked_device_type[event.trackedDeviceIndex] = "";
        }
            break;
        case vr::VREvent_TrackedDeviceUpdated: {
            cout << "Device " << event.trackedDeviceIndex << " updated (" << str_td_class << ")" << endl;
        }
            break;
        case vr::VREvent_ButtonPress: {
            vr::VREvent_Controller_t controller_data = event.data.controller;
            cout << "Pressed button " << vr_context->GetButtonIdNameFromEnum((vr::EVRButtonId) controller_data.button)
                 << " of device " << event.trackedDeviceIndex << " (" << str_td_class << ")" << endl;
        }
            break;
        case vr::VREvent_ButtonUnpress: {
            vr::VREvent_Controller_t controller_data = event.data.controller;
            cout << "Unpressed button " << vr_context->GetButtonIdNameFromEnum((vr::EVRButtonId) controller_data.button)
                 << " of device " << event.trackedDeviceIndex << " (" << str_td_class << ")" << endl;
        }
            break;
        case vr::VREvent_ButtonTouch: {
            vr::VREvent_Controller_t controller_data = event.data.controller;
            cout << "Touched button " << vr_context->GetButtonIdNameFromEnum((vr::EVRButtonId) controller_data.button)
                 << " of device " << event.trackedDeviceIndex << " (" << str_td_class << ")" << endl;
        }
            break;
        case vr::VREvent_ButtonUntouch: {
            vr::VREvent_Controller_t controller_data = event.data.controller;
            cout << "Untouched button " << vr_context->GetButtonIdNameFromEnum((vr::EVRButtonId) controller_data.button)
                 << " of device " << event.trackedDeviceIndex << " (" << str_td_class << ")" << endl;
        }
            break;
    }
}

void exit() {
    vr::VR_Shutdown();

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}