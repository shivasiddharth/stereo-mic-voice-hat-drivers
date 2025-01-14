/*
 * Simple driver for Hifiberry and ICS43432 MEMS microphone compatible hardware
 *
 * Author:	Greg Miell <greg@gothack.ninja>
 *		Copyright 2017
 * Based on googlevoicehat-codec.c by Peter Malkin
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#include <linux/device.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/version.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/soc-dai.h>
#include <sound/soc-dapm.h>

static int stereohat_codec_probe(struct snd_soc_codec *codec) {
  return 0;
}

static int stereohat_codec_remove(struct snd_soc_codec *codec) {
  return 0;
}

static const struct snd_soc_dapm_widget stereohat_dapm_widgets[] = {
    SND_SOC_DAPM_OUTPUT("Speaker"),
};

static const struct snd_soc_dapm_route stereohat_dapm_routes[] = {
    {"Speaker", NULL, "Playback"},
};

static struct snd_soc_codec_driver stereohat_codec_driver = {
    .probe = stereohat_codec_probe,
    .remove = stereohat_codec_remove,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 9, 0)
    .component_driver = {
#endif
    .dapm_widgets = stereohat_dapm_widgets,
    .num_dapm_widgets = ARRAY_SIZE(stereohat_dapm_widgets),
    .dapm_routes = stereohat_dapm_routes,
    .num_dapm_routes = ARRAY_SIZE(stereohat_dapm_routes),
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 9, 0)
    },
#endif
};

static struct snd_soc_dai_driver stereohat_dai = {
    .name = "stereo-audio-hat",
    .capture = {.stream_name = "Capture",
                .channels_min = 2,
                .channels_max = 2,
                .rates = SNDRV_PCM_RATE_48000,
                .formats = SNDRV_PCM_FMTBIT_S32_LE},
    .playback = {.stream_name = "Playback",
                 .channels_min = 2,
                 .channels_max = 2,
                 .rates = SNDRV_PCM_RATE_48000,
                 .formats = SNDRV_PCM_FMTBIT_S32_LE},
    .symmetric_rates = 1};

#ifdef CONFIG_OF
static const struct of_device_id stereohat_ids[] = {
    {
        .compatible = "stereohat,stereohat",
    },
    {}};
MODULE_DEVICE_TABLE(of, stereohat_ids);
#endif

static int stereohat_platform_probe(struct platform_device *pdev) {
  return snd_soc_register_codec(&pdev->dev, &stereohat_codec_driver, &stereohat_dai, 1);
}

static int stereohat_platform_remove(struct platform_device *pdev) {
  snd_soc_unregister_codec(&pdev->dev);
  return 0;
}

static struct platform_driver stereohat_driver = {
    .driver =
        {
            .name = "stereohat-codec", .of_match_table = of_match_ptr(stereohat_ids),
        },
    .probe = stereohat_platform_probe,
    .remove = stereohat_platform_remove,
};

module_platform_driver(stereohat_driver);

MODULE_DESCRIPTION("Simple I2S Hifiberry & ICS43432 MEMS Codec");
MODULE_AUTHOR("Greg Miell <greg@gothack.ninja>");
MODULE_LICENSE("GPL v2");
