deps_config := \
	/home/sumedh/esp/esp-idf/components/app_trace/Kconfig \
	/home/sumedh/esp/esp-idf/components/aws_iot/Kconfig \
	/home/sumedh/esp/esp-idf/components/bt/Kconfig \
	/home/sumedh/esp/esp-idf/components/driver/Kconfig \
	/home/sumedh/esp/esp-idf/components/esp32/Kconfig \
	/home/sumedh/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/home/sumedh/esp/esp-idf/components/esp_http_client/Kconfig \
	/home/sumedh/esp/esp-idf/components/ethernet/Kconfig \
	/home/sumedh/esp/esp-idf/components/fatfs/Kconfig \
	/home/sumedh/esp/esp-idf/components/freertos/Kconfig \
	/home/sumedh/esp/esp-idf/components/heap/Kconfig \
	/home/sumedh/esp/esp-idf/components/libsodium/Kconfig \
	/home/sumedh/esp/esp-idf/components/log/Kconfig \
	/home/sumedh/esp/esp-idf/components/lwip/Kconfig \
	/home/sumedh/esp/esp-idf/components/mbedtls/Kconfig \
	/home/sumedh/esp/esp-idf/components/mdns/Kconfig \
	/home/sumedh/esp/esp-idf/components/openssl/Kconfig \
	/home/sumedh/esp/esp-idf/components/pthread/Kconfig \
	/home/sumedh/esp/esp-idf/components/spi_flash/Kconfig \
	/home/sumedh/esp/esp-idf/components/spiffs/Kconfig \
	/home/sumedh/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/sumedh/esp/esp-idf/components/vfs/Kconfig \
	/home/sumedh/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/sumedh/esp/esp-idf/Kconfig.compiler \
	/home/sumedh/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/sumedh/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/sumedh/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/sumedh/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
