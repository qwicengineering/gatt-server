gatt-server
===========

This module helps you create a GATT server in the ESP32 using PlatformIO. It's an example on how to create a BLE profile that contains many services.

The library comes with a service already created as an example called "dashboard service". It is designed to create an app that works as the dashboard for a simple electric vehicle (it contains characteristics to share data like speed, battery charge, odometer...).

There is also a template folder to simplify the creation of new services.




Overview
--------

```
ble-communication/
├── examples
│   └── dashboard <--- example of how to use the dashboard service that comes already defined in the library
│
├── ble-communication.c <--- Entry point for the library
├── ble-communication.h
├── ble-configuration.c <--- Configuration for the driver, connection, and advertising data
├── ble-configuration.h
├── gap-event-handler.c <--- Treats the events generated by the GAP protocol
├── gap-event-handler.h
├── gatt-profile
│   ├── gatt-profile-api.h <--- Exposes the public functions of the different services and their configuration structures
│   ├── gatt-profile.c <--- Creates the profile table, treats or redirects GATT events and initializes the services
│   ├── gatt-profile.h
│   └── services
│       ├── services-declarations.c 
│       ├── services-declarations.h <--- Macros, defines and variables shared among the services
│       ├── dashboard-service
│       │   ├── dashboard-service.c <--- Implementation of the public interface of the service
│       │   ├── dashboard-service.h 
│       │   ├── dashboard-service-database.c <--- Definition of the service table, structure to hold the data and UUID's
│       │   ├── dashboard-service-database.h 
│       │   ├── dashboard-service-event-handler.c <--- Treats the GATT events related to this service
│       │   └── dashboard-service-event-handler.h
│       │
│       └── service-template <--- Template files to create your own service following the same pattern
└── README.md

```


Creating a new service
----------------------
I suggest to start by making a copy of the service-template folder and proceed to rename every file (remember the includes!) on it. You'll need to modify the content of every file, hopefully, each section has a good enough explanation on its own.

Pay special attention to the service database. That array is directly used by the ESP-IDF library. The library implementation is quite close to the BLE definition itself so if you have doubts about what something means, I recommend going straight to the BLE definitions.

Once you have implemented your service you need to include it in the profile, for that you'll need to modify `gatt-profile-api.h`, `gatt-profile.c`, and `gatt-profile.h`. 

In `gatt-profile.h` you need to edit the number of services definition and the enumeration that contain the indexes for the profile table.
In `gatt-profile-api.h` you need to include your `XXX-service.h` and edit `gatt_profile_config_t` if you need to pass any configuration parameter to your service.
Finally, in `gatt-profile.c` you need to 
 - Include your `XXX-service-database.h` and `XXX-service-event-handler.h`
 - Modify the declaration of `gatt_profile_tab` to include your new service
 - Modify `gatt_profile_init` to call the initialization of your service (you can also modify the advertising info here).

