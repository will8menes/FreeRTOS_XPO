idf_component_get_property(mb_dir freemodbus COMPONENT_DIR)

idf_component_register(SRCS "pzem_service.c"
                    INCLUDE_DIRS "include"
                    "${mb_dir}/common/include" 
                    "${mb_dir}/common" 
                    "${mb_dir}/port" 
                    "${mb_dir}/modbus/include" 
                    "${mb_dir}/modbus/rtu"
                    )

