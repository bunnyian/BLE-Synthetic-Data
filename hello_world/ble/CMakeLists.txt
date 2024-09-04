target_include_directories(app PRIVATE inc)

target_sources(app PRIVATE src/ble_service.c)
target_sources(app PRIVATE src/comm_protocol.c)
target_sources(app PRIVATE src/synth_data.c)