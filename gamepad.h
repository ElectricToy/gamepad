/*
 * Written by Simon Fuhrmann.
 * See LICENSE file for details.
 */
#ifndef GAMEPAD_HEADER
#define GAMEPAD_HEADER

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace gamepad {

struct Device {
  unsigned int device_id = 0;
  int vendor_id = 0;
  int product_id = 0;
  std::string description;
  std::vector<float> axes;
  std::vector<bool> buttons;
};

class System {
 public:
  // The attached handler signature.
  typedef std::function<void(Device*)> AttachedHandler;
  // The detached handler signature.
  typedef std::function<void(Device*)> DetachedHandler;
  // The button handler signature (device, button ID, timestamp).
  typedef std::function<void(Device*, int, double)> ButtonHandler;
  // The axis handler signature (device, axis ID, value, old value, timestamp).
  typedef std::function<void(Device*, int, float, float, double)> AxisHandler;

 public:
  static std::unique_ptr<System> Create();
  virtual ~System() = default;

  // Registers a handler that is called when a pad is attached.
  void RegisterAttachHandler(AttachedHandler handler);
  // Registers a handler that is called when a pad is detached.
  void RegisterDetachHandler(DetachedHandler handler);
  // Registers a handler for button down events.
  void RegisterButtonDownHandler(ButtonHandler handler);
  // Registers a handler for button up events.
  void RegisterButtonUpHandler(ButtonHandler handler);
  // Registers a handler for axis move events.
  void RegisterAxisMoveHandler(AxisHandler handler);

  // Processes all events and invokes the corresponding handler functions.
  virtual void ProcessEvents() = 0;

  // Scans for new devices and invokes the attach handler for each new device.
  // The cost of this call depends on the implementation.
  // MacOS: Essentially free, devices are attached using IOKit callbacks.
  // Linux: Needs to scan /dev/input for devices not already attached.
  virtual void ScanForDevices() = 0;

 protected:
  System() = default;
  void HandleButtonEvent(Device* device, int button_id, int value);
  void HandleAxisEvent(Device* device, int axis_id, int value,
      int min, int max, int fuzz, int flat);

  AttachedHandler attached_handler_;
  DetachedHandler detached_handler_;
  ButtonHandler button_up_handler_;
  ButtonHandler button_down_handler_;
  AxisHandler axis_move_handler_;
};

}  // namespace pad

#endif  // GAMEPAD_HEADER
