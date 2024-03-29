/*
 * mcp4822.h
 *
 *  Created on: Aug 12, 2023
 *      Author: hwang
 */

#ifndef SRC_HW_DRIVER_SPI_MCP48X2_2_H_
#define SRC_HW_DRIVER_SPI_MCP48X2_2_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* constants for channel */
typedef enum
{
	MCP48X2_DAC_CH_A = 0,
	MCP48X2_DAC_CH_B = 1,
} mcp48x2_ch_t;

/* constants for channel mode */
typedef enum
{
	MCP48X2_CH_SHDN = 0,
	MCP48X2_CH_ACTIVE = 1,
} mcp48x2_ch_mode_t;

/* constants for gain factor for output voltage */
typedef enum
{
	MCP48X2_GAIN_2X = 0,
	MCP48X2_GAIN_1X = 1,
} mcp48x2_gain_t;

/* Possible return values */
typedef enum
{
	MCP48X2_OK = 0,
	MCP48X2_FAIL = 1,
} mcp48x2_ret_t;

/* Low level functions to ensure SPI, NSS and LDAC managing */
typedef struct
{
	/* Manage NSS pin */
	void (*nss_set)(void);
	void (*nss_reset)(void);

	/* Manage LDAC pin */
	void (*ldac_set)(void);
	void (*ldac_reset)(void);
	void (*delay_us)(uint32_t us);

	/* Transmit 16 bits with SPI */
	bool (*spi_write_packet)(uint8_t ch, uint16_t data);
} mcp48x2_ll_t;

/* One instance of DAC */
typedef struct
{
	mcp48x2_ch_mode_t mode_ch_a;
	mcp48x2_ch_mode_t mode_ch_b;
	mcp48x2_gain_t gain_ch_a;
	mcp48x2_gain_t gain_ch_b;
	uint16_t ch_a_val;
	uint16_t ch_b_val;
	mcp48x2_ll_t *ll;
} mcp48x2_device_t;

/**
 * Default setup:
 * both channels are in active mode, gain factor equals 2
 * and initial output voltages are set to zero.
 *
 * Also the function fill in mcp48x2_device_t structure with
 * corresponding values.
 *
 * Function returns 'MCP48X2_OK' in case of success.
 */
mcp48x2_ret_t mcp48x2_default_init(mcp48x2_device_t *dev, mcp48x2_ll_t *ll);

/**
 * Init particular channel. Call this function for each channel.
 * If you use this function for channels, don't use the 'mcp48x2_default_init()'.
 *
 * Function returns 'MCP48X2_OK' in case of success.
 */
mcp48x2_ret_t mcp48x2_init_channel(mcp48x2_device_t *dev, mcp48x2_ll_t *ll,
                                   mcp48x2_ch_t ch, mcp48x2_ch_mode_t mode,
                                   mcp48x2_gain_t gain);

/**
 * Set channel mode of work to active or shutdown.
 *
 * Function returns 'MCP48X2_OK' in case of success.
 */
mcp48x2_ret_t mcp48x2_set_channel_mode(mcp48x2_device_t *dev,
                                       mcp48x2_ch_t ch,
                                       mcp48x2_ch_mode_t mode);

/**
 * Set channel gain factor to 1x or 2x.
 *
 * Function returns 'MCP48X2_OK' in case of success.
 */
mcp48x2_ret_t mcp48x2_set_channel_gain(mcp48x2_device_t *dev,
                                       mcp48x2_ch_t ch,
                                       mcp48x2_gain_t gain);

/**
 * Write value to output register for one channel and toggle the LDAC pin.
 * Other channel is stayed unchangeable
 *
 * Formula: Vout = (2048 * val * G) / 2^(resolution)
 *
 * 	2048 - internal reference voltage
 * 	G - gain factor
 * 	val = [0 - 4095] for MCP4922
 *	val = [0 - 1023] for MCP4912
 *	val = [0 - 255]  for MCP4902
 *
 * Function returns 'MCP48X2_OK' in case of success.
 */
mcp48x2_ret_t mcp48x2_set_channel_value(mcp48x2_device_t *dev,
                                        mcp48x2_ch_t ch, uint16_t val);

/**
 * Write values to output registers for both channels
 * and toggle the LDAC pin to synchronize setting the values.
 *
 * Formula: Vout = (2048 * val * G) / 2^(resolution)
 *
 * 	2048 - internal reference voltage
 *	G - gain factor
 * 	val = [0 - 4095] for MCP4922
 *	val = [0 - 1023] for MCP4912
 *	val = [0 - 255]  for MCP4902
 *
 * Function returns 'MCP48X2_OK' in case of success.
 */
mcp48x2_ret_t mcp48x2_set_channel_values_sync(mcp48x2_device_t *dev,
                                              uint16_t val_ch_a,
                                              uint16_t val_ch_b);

#ifdef __cplusplus
}
#endif



#endif /* SRC_HW_DRIVER_SPI_MCP48X2_2_H_ */
