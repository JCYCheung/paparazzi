#ifndef FLIGHT_GEAR_H
#define FLIGHT_GEAR_H

#include <stdint.h>

#define FG_NET_CTRLS_VERSION 26
#define FG_NET_CTRLS_MAX_ENGINES 4
#define FG_NET_CTRLS_MAX_WHEELS  16
#define FG_NET_CTRLS_MAX_TANKS   6

struct FGNetCtrls {
  uint32_t version;                    // increment when data values change

  // Aero controls
  double aileron;                      // -1 ... 1
  double elevator;                     // -1 ... 1
  double rudder;                       // -1 ... 1
  double aileron_trim;                 // -1 ... 1
  double elevator_trim;                // -1 ... 1
  double rudder_trim;                  // -1 ... 1
  double flaps;                        //  0 ... 1

  // Aero control faults
  uint32_t flaps_power;                 // true = power available
  uint32_t flap_motor_ok;

  // Engine controls
  uint32_t num_engines;                // number of valid engines
  uint32_t master_bat[FG_NET_CTRLS_MAX_ENGINES];
  uint32_t master_alt[FG_NET_CTRLS_MAX_ENGINES];
  uint32_t magnetos[FG_NET_CTRLS_MAX_ENGINES];
  uint32_t starter_power[FG_NET_CTRLS_MAX_ENGINES];// true = starter power
  double throttle[FG_NET_CTRLS_MAX_ENGINES];     //  0 ... 1
  double mixture[FG_NET_CTRLS_MAX_ENGINES];      //  0 ... 1
  double condition[FG_NET_CTRLS_MAX_ENGINES];    //  0 ... 1
  uint32_t fuel_pump_power[FG_NET_CTRLS_MAX_ENGINES];// true = on
  double prop_advance[FG_NET_CTRLS_MAX_ENGINES]; //  0 ... 1

  // Engine faults
  uint32_t engine_ok[FG_NET_CTRLS_MAX_ENGINES];
  uint32_t mag_left_ok[FG_NET_CTRLS_MAX_ENGINES];
  uint32_t mag_right_ok[FG_NET_CTRLS_MAX_ENGINES];
  uint32_t spark_plugs_ok[FG_NET_CTRLS_MAX_ENGINES];  // false = fouled plugs
  uint32_t oil_press_status[FG_NET_CTRLS_MAX_ENGINES];// 0 = normal, 1 = low, 2 = full fail
  uint32_t fuel_pump_ok[FG_NET_CTRLS_MAX_ENGINES];

  // Fuel management
  uint32_t num_tanks;                      // number of valid tanks
  uint32_t fuel_selector[FG_NET_CTRLS_MAX_TANKS];    // false = off, true = on
  uint32_t cross_feed;                     // false = off, true = on

  // Brake controls
  double brake_left;
  double brake_right;
  double copilot_brake_left;
  double copilot_brake_right;
  double brake_parking;

  // Landing Gear
  uint32_t gear_handle; // true=gear handle down; false= gear handle up

  // Switches
  uint32_t master_avionics;

  // wind and turbulance
  double wind_speed_kt;
  double wind_dir_deg;
  double turbulence_norm;

  // temp and pressure
  double temp_c;
  double press_inhg;

  // other information about environment
  double hground;                      // ground elevation (meters)
  double magvar;                       // local magnetic variation in degs.

  // hazards
  uint32_t icing;                      // icing status could me much
  // more complex but I'm
  // starting simple here.

  // simulation control
  uint32_t speedup;                    // integer speedup multiplier
  uint32_t freeze;                     // 0=normal
                                       // 0x01=master
                                       // 0x02=position
                                       // 0x04=fuel

};


#define FG_NET_FDM_VERSION 23
#define FG_NET_FDM_MAX_ENGINES 4
#define FG_NET_FDM_MAX_WHEELS  3
#define FG_NET_FDM_MAX_TANKS   4

#ifndef _NET_FDM_HXX

struct FGNetFDM {

  uint32_t version;           // increment when data values change
  uint32_t padding;           // padding

  // Positions
  double longitude;           // geodetic (radians)
  double latitude;            // geodetic (radians)
  double altitude;            // above sea level (meters)
  float agl;                  // above ground level (meters)
  float phi;                  // roll (radians)
  float theta;                // pitch (radians)
  float psi;                  // yaw or true heading (radians)
  float alpha;                // angle of attack (radians)
  float beta;                 // side slip angle (radians)

  // Velocities
  float phidot;               // roll rate (radians/sec)
  float thetadot;             // pitch rate (radians/sec)
  float psidot;               // yaw rate (radians/sec)
  float vcas;                 // calibrated airspeed
  float climb_rate;           // feet per second
  float v_north;              // north velocity in local/body frame, fps
  float v_east;               // east velocity in local/body frame, fps
  float v_down;               // down/vertical velocity in local/body frame, fps
  float v_wind_body_north;    // north velocity in local/body frame
                              // relative to local airmass, fps
  float v_wind_body_east;     // east velocity in local/body frame
                              // relative to local airmass, fps
  float v_wind_body_down;     // down/vertical velocity in local/body
                              // frame relative to local airmass, fps

  // Accelerations
  float A_X_pilot;            // X accel in body frame ft/sec^2
  float A_Y_pilot;            // Y accel in body frame ft/sec^2
  float A_Z_pilot;            // Z accel in body frame ft/sec^2
  // Stall
  float stall_warning;        // 0.0 - 1.0 indicating the amount of stall
  float slip_deg;             // slip ball deflection

  // Pressure

  // Engine status
  uint32_t num_engines;            // Number of valid engines
  uint32_t eng_state[FG_NET_FDM_MAX_ENGINES];// Engine state (off, cranking, running)
  float rpm[FG_NET_FDM_MAX_ENGINES];       // Engine RPM rev/min
  float fuel_flow[FG_NET_FDM_MAX_ENGINES]; // Fuel flow gallons/hr
  float egt[FG_NET_FDM_MAX_ENGINES];       // Exhuast gas temp deg F
  float cht[FG_NET_FDM_MAX_ENGINES];       // Cylinder head temp deg F
  float mp_osi[FG_NET_FDM_MAX_ENGINES];    // Manifold pressure
  float tit[FG_NET_FDM_MAX_ENGINES];       // Turbine Inlet Temperature
  float oil_temp[FG_NET_FDM_MAX_ENGINES];  // Oil temp deg F
  float oil_px[FG_NET_FDM_MAX_ENGINES];    // Oil pressure psi

  // Consumables
  uint32_t num_tanks;         // Max number of fuel tanks
  float fuel_quantity[FG_NET_FDM_MAX_TANKS];

  // Gear status
  uint32_t num_wheels;
  uint32_t wow[FG_NET_FDM_MAX_WHEELS];
  float gear_pos[FG_NET_FDM_MAX_WHEELS];
  float gear_steer[FG_NET_FDM_MAX_WHEELS];
  float gear_compression[FG_NET_FDM_MAX_WHEELS];

  // Environment
  uint32_t cur_time;           // current unix time
  // FIXME: make this uint64_t before 2038
  int32_t warp;                // offset in seconds to unix time
  float visibility;            // visibility in meters (for env. effects)

  // Control surface positions (normalized values)
  float elevator;
  float elevator_trim_tab;
  float left_flap;
  float right_flap;
  float left_aileron;
  float right_aileron;
  float rudder;
  float nose_wheel;
  float speedbrake;
  float spoilers;
};

#endif

struct FGNetMiniFDM {
   uint32_t version;           // increment when data values change

    // Positions
    double longitude;           // geodetic (radians)
    double latitude;            // geodetic (radians)
    double altitude;            // above sea level (meters)
    double agl;                 // above ground level (meters)
    double phi;                 // roll (radians)
    double theta;               // pitch (radians)
    double psi;                 // yaw or true heading (radians)

    // Velocities
    double vcas;
    double climb_rate;          // feet per second

    // Consumables
    uint32_t num_tanks;         // Max number of fuel tanks
    double fuel_quantity[FG_NET_FDM_MAX_TANKS];

    // Environment
    uint32_t cur_time;            // current unix time
    int32_t warp;                 // offset in seconds to unix time
};

#define FG_NET_GUI_VERSION 7
#define FG_NET_GUI_MAX_TANKS 4

// FIXME: Flightgear on OSX is still 32 bit get rid
// off these pragmas when it goes to 64 bit.
#ifdef __x86_64__
#pragma pack(push)
#ifdef __APPLE__
#pragma pack(4)
#else
#pragma pack(8)
#endif
#endif
struct FGNetGUI {
  uint32_t version;           // increment when data values change

  // Positions
  double longitude;           // geodetic (radians)
  double latitude;            // geodetic (radians)
  float altitude;             // above sea level (meters)
  float agl;                  // above ground level (meters)
  float phi;                  // roll (radians)
  float theta;                // pitch (radians)
  float psi;                  // yaw or true heading (radians)

  // Velocities
  float vcas;
  float climb_rate;           // feet per second

  // Consumables
  uint32_t num_tanks;         // Max number of fuel tanks
  float fuel_quantity[FG_NET_GUI_MAX_TANKS];

  // Environment
  uint32_t cur_time;          // current unix time
                                // FIXME: make this uint64_t before 2038
  uint32_t warp;              // offset in seconds to unix time
  float ground_elev;          // ground elev (meters)

  // Approach
  float tuned_freq;           // currently tuned frequency
  float nav_radial;           // target nav radial
  uint32_t in_range;           // tuned navaid is in range?
  float dist_nm;              // distance to tuned navaid in nautical miles
  float course_deviation_deg; // degrees off target course
  float gs_deviation_deg;     // degrees off target glide slope
};
#ifdef __x86_64__
#pragma pack(push)
#pragma pack(pop)
#endif


extern void net_fdm_dump (struct FGNetFDM* fdm);
extern void net_fdm_ntoh (struct FGNetFDM* fdm);
extern void net_fdm_init (struct FGNetFDM* fdm);

extern void net_gui_init (struct FGNetGUI* gui);
extern void net_gui_hton (struct FGNetGUI* gui);
extern void net_gui_dump (struct FGNetGUI* gui);

extern void net_ctrls_dump(struct FGNetCtrls* ctrls);
extern void net_ctrls_ntoh(struct FGNetCtrls* ctrls);

#endif /* FLIGHT_GEAR_H */
