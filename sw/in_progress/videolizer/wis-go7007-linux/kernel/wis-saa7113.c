/*
 * Copyright (C) 2005-2006 Micronas USA Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (Version 2) as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/i2c.h>
#include <linux/videodev.h>
#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/video_decoder.h> 

#include "wis-i2c.h"

struct wis_saa7113 {
	int norm;
	int brightness;
	int contrast;
	int saturation;
	int hue;
};

static u8 initial_registers[]=
{
	0x01, 0x08,
	0x02, 0xc0,
	0x03, 0x33,
	0x04, 0x00,
	0x05, 0x00,
	0x06, 0xe9,
	0x07, 0x0d,
	0x08, 0xd8,
	0x09, 0x40,
	0x0a, 0x80,
	0x0b, 0x47,
	0x0c, 0x40,
	0x0d, 0x00,
	0x0e, 0x01,
	0x0f, 0x2a,
	0x10, 0x40,
	0x11, 0x0c,
	0x12, 0xfe,
	0x13, 0x00,
	0x14, 0x00,
	0x15, 0x04,
	0x16, 0x00,
	0x17, 0x00,
	0x18, 0x00,
	0x19, 0x00,
	0x1a, 0x00,
	0x1b, 0x00,
	0x1c, 0x00,
	0x1d, 0x00,
	0x1e, 0x00,
	0x1f, 0xc8,
	0x40, 0x00,
	0x41, 0xff,
	0x42, 0xff,
	0x43, 0xff,
	0x44, 0xff,
	0x45, 0xff,
	0x46, 0xff,
	0x47, 0xff,
	0x48, 0xff,
	0x49, 0xff,
	0x4a, 0xff,
	0x4b, 0xff,
	0x4c, 0xff,
	0x4d, 0xff,
	0x4e, 0xff,
	0x4f, 0xff,
	0x50, 0xff,
	0x51, 0xff,
	0x52, 0xff,
	0x53, 0xff,
	0x54, 0xff,
	0x55, 0xff,
	0x56, 0xff,
	0x57, 0xff,
	0x58, 0x00,
	0x59, 0x54,
	0x5a, 0x07,
	0x5b, 0x83,
	0x5c, 0x00,
	0x5d, 0x00,
	0x5e, 0x00,
	0x5f, 0x00,
	0x60, 0x00,
	0x61, 0x00,
	0x00, 0x00, /* Terminator (reg 0x00 is read-only) */
};

static int write_reg(struct i2c_client *client, u8 reg, u8 value)
{
	return i2c_smbus_write_byte_data(client, reg, value);
}

static int write_regs(struct i2c_client *client, u8 *regs)
{
	int i;

	for (i = 0; regs[i] != 0x00; i += 2)
		if (i2c_smbus_write_byte_data(client, regs[i], regs[i + 1]) < 0)
			return -1;
	return 0;
}

static int wis_saa7113_command(struct i2c_client *client,
				unsigned int cmd, void *arg)
{
	struct wis_saa7113 *dec = i2c_get_clientdata(client);

	switch (cmd) {
	case DECODER_SET_INPUT:
	{
		int *input = arg;

		i2c_smbus_write_byte_data(client, 0x02, 0xC0 | *input);
		i2c_smbus_write_byte_data(client, 0x09,
				*input < 6 ? 0x40 : 0x80);
		break;
	}
	case DECODER_SET_NORM:
	{
		int *input = arg;
		dec->norm = *input;
		switch (dec->norm) {
		case VIDEO_MODE_PAL:
			write_reg(client, 0x0e, 0x01);
			write_reg(client, 0x10, 0x48);
			break;
		case VIDEO_MODE_NTSC:
			write_reg(client, 0x0e, 0x01);
			write_reg(client, 0x10, 0x40);
			break;
		case VIDEO_MODE_SECAM:
			write_reg(client, 0x0e, 0x50);
			write_reg(client, 0x10, 0x48);
			break;
		}
		break;
	}
	case VIDIOC_QUERYCTRL:
	{
		struct v4l2_queryctrl *ctrl = arg;

		switch (ctrl->id) {
		case V4L2_CID_BRIGHTNESS:
			ctrl->type = V4L2_CTRL_TYPE_INTEGER;
			strncpy(ctrl->name, "Brightness", sizeof(ctrl->name));
			ctrl->minimum = 0;
			ctrl->maximum = 255;
			ctrl->step = 1;
			ctrl->default_value = 128;
			ctrl->flags = 0;
			break;
		case V4L2_CID_CONTRAST:
			ctrl->type = V4L2_CTRL_TYPE_INTEGER;
			strncpy(ctrl->name, "Contrast", sizeof(ctrl->name));
			ctrl->minimum = 0;
			ctrl->maximum = 127;
			ctrl->step = 1;
			ctrl->default_value = 71;
			ctrl->flags = 0;
			break;
		case V4L2_CID_SATURATION:
			ctrl->type = V4L2_CTRL_TYPE_INTEGER;
			strncpy(ctrl->name, "Saturation", sizeof(ctrl->name));
			ctrl->minimum = 0;
			ctrl->maximum = 127;
			ctrl->step = 1;
			ctrl->default_value = 64;
			ctrl->flags = 0;
			break;
		case V4L2_CID_HUE:
			ctrl->type = V4L2_CTRL_TYPE_INTEGER;
			strncpy(ctrl->name, "Hue", sizeof(ctrl->name));
			ctrl->minimum = -128;
			ctrl->maximum = 127;
			ctrl->step = 1;
			ctrl->default_value = 0;
			ctrl->flags = 0;
			break;
		}
		break;
	}
	case VIDIOC_S_CTRL:
	{
		struct v4l2_control *ctrl = arg;

		switch (ctrl->id) {
		case V4L2_CID_BRIGHTNESS:
			if (ctrl->value > 255)
				dec->brightness = 255;
			else if (ctrl->value < 0)
				dec->brightness = 0;
			else
				dec->brightness = ctrl->value;
			write_reg(client, 0x0a, dec->brightness);
			break;
		case V4L2_CID_CONTRAST:
			if (ctrl->value > 127)
				dec->contrast = 127;
			else if (ctrl->value < 0)
				dec->contrast = 0;
			else
				dec->contrast = ctrl->value;
			write_reg(client, 0x0b, dec->contrast);
			break;
		case V4L2_CID_SATURATION:
			if (ctrl->value > 127)
				dec->saturation = 127;
			else if (ctrl->value < 0)
				dec->saturation = 0;
			else
				dec->saturation = ctrl->value;
			write_reg(client, 0x0c, dec->saturation);
			break;
		case V4L2_CID_HUE:
			if (ctrl->value > 127)
				dec->hue = 127;
			else if (ctrl->value < -128)
				dec->hue = -128;
			else
				dec->hue = ctrl->value;
			write_reg(client, 0x0d, dec->hue);
			break;
		}
		break;
	}
	case VIDIOC_G_CTRL:
	{
		struct v4l2_control *ctrl = arg;

		switch (ctrl->id) {
		case V4L2_CID_BRIGHTNESS:
			ctrl->value = dec->brightness;
			break;
		case V4L2_CID_CONTRAST:
			ctrl->value = dec->contrast;
			break;
		case V4L2_CID_SATURATION:
			ctrl->value = dec->saturation;
			break;
		case V4L2_CID_HUE:
			ctrl->value = dec->hue;
			break;
		}
		break;
	}
	default:
		break;
	}
	return 0;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,12)
static int wis_saa7113_i2c_id = 0;
#endif
static struct i2c_driver wis_saa7113_driver;

static struct i2c_client wis_saa7113_client_templ = {
	.name		= "SAA7113 (WIS)",
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,16)
	.flags		= I2C_CLIENT_ALLOW_USE,
#endif
	.driver		= &wis_saa7113_driver,
};

static int wis_saa7113_detect(struct i2c_adapter *adapter, int addr, int kind)
{
	struct i2c_client *client;
	struct wis_saa7113 *dec;

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		return 0;

	client = kmalloc(sizeof(struct i2c_client), GFP_KERNEL);
	if (client == NULL)
		return -ENOMEM;
	memcpy(client, &wis_saa7113_client_templ,
			sizeof(wis_saa7113_client_templ));
	client->adapter = adapter;
	client->addr = addr;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,12)
	client->id = wis_saa7113_i2c_id++;
#endif

	dec = kmalloc(sizeof(struct wis_saa7113), GFP_KERNEL);
	if (dec == NULL) {
		kfree(client);
		return -ENOMEM;
	}
	dec->norm = VIDEO_MODE_NTSC;
	dec->brightness = 128;
	dec->contrast = 71;
	dec->saturation = 64;
	dec->hue = 0;
	i2c_set_clientdata(client, dec);

	printk(KERN_DEBUG
		"wis-saa7113: initializing SAA7113 at address %d on %s\n",
		addr, adapter->name);

	if (write_regs(client, initial_registers) < 0)
	{
		printk(KERN_ERR
			"wis-saa7113: error initializing SAA7113\n");
		kfree(client);
		kfree(dec);
		return 0;
	}

	i2c_attach_client(client);
	return 0;
}

static int wis_saa7113_detach(struct i2c_client *client)
{
	struct wis_saa7113 *dec = i2c_get_clientdata(client);
	int r;

	r = i2c_detach_client(client);
	if (r < 0)
		return r;

	kfree(client);
	kfree(dec);
	return 0;
}

static struct i2c_driver wis_saa7113_driver = {
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,16)
	.owner		= THIS_MODULE,
	.name		= "WIS SAA7113 I2C driver",
#else
	.driver = {
		.name	= "WIS SAA7113 I2C driver",
	},
#endif
	.id		= I2C_DRIVERID_WIS_SAA7113,
	.detach_client	= wis_saa7113_detach,
	.command	= wis_saa7113_command,
};

static int __init wis_saa7113_init(void)
{
	int r;

	r = i2c_add_driver(&wis_saa7113_driver);
	if (r < 0)
		return r;
	return wis_i2c_add_driver(wis_saa7113_driver.id, wis_saa7113_detect);
}

static void __exit wis_saa7113_cleanup(void)
{
	wis_i2c_del_driver(wis_saa7113_detect);
	i2c_del_driver(&wis_saa7113_driver);
}

module_init(wis_saa7113_init);
module_exit(wis_saa7113_cleanup);

MODULE_LICENSE("GPL v2");