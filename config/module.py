def loadModule():
	#print("Load Module: Harmony USB Middle ware")
	
	# Create USB Driver Components 
	if any(x in Variables.get("__PROCESSOR") for x in ["SAMV70", "SAMV71", "SAME70", "SAMS70"]):
		print("create component: USB Driver")
		usbDriverComponent =  Module.CreateComponent("drv_usbhs_v1", "USB High Speed Driver", "/Harmony/Drivers", "config/usb_driver.py")
		usbDriverComponent.addCapability("DRV_USB", "DRV_USB",True)
	
	if any(x in Variables.get("__PROCESSOR") for x in ["SAMD20", "SAMD21", "SAMD51", "SAME51", "SAME53", "SAME54"]):
		print("create component: USB Driver")
		usbDriverComponent =  Module.CreateComponent("drv_usbfs_v1", "USB Full Speed Driver", "/Harmony/Drivers", "config/usbfs_v1_driver.py")
		usbDriverComponent.addCapability("DRV_USB", "DRV_USB",True)
		
	# Create USB Device Layer Component 
	usbDeviceComponent = Module.CreateSharedComponent("usb_device", "USB Device Layer", "/Libraries/USB/Device Stack", "config/usb_device.py")
	usbDeviceComponent.addDependency("usb_driver_dependency", "DRV_USB", True, True)
	usbDeviceComponent.addCapability("USB Device", "USB_DEVICE", True)
	
	# Create USB Device Function Driver Components 
	# usbDeviceFunctionComponents = [{"name":"usbDeviceCdcComponent", "label": "CDC", "type":"driver", "instance":"multi", "capability":["USB_DEVICE_CDC"], "dependency":["UART"], "condition":"True"},
								   # {"name":"usbDeviceHidComponent", "label": "HID", "type":"driver", "instance":"multi", "capability":["USB_DEVICE_HID"], "dependency":["MEMORY"], "condition":"True"}
								  # ]
	
	# # Load USB Device Function driver components from the above list  
	# for usbDeviceFunctionComponent in usbDeviceFunctionComponents:

    # #check if component should be created
    # if eval(coreComponent['condition']):
        # Name = coreComponent['name']
        # Label = coreComponent['label']

        # #create system component
        # if coreComponent['type'] == "system":
            # print("create component: " + Name.upper() + " System Service")
            # Component = Module.CreateSharedComponent("sys_" + Name, Label, "/Harmony/System Services", "system/" + Name + "/config/sys_" + Name + ".py")

            # if "capability" in coreComponent:
                # for capability in coreComponent['capability']:
                    # Component.addCapability(capability.lower(), capability)

            # if "dependency" in coreComponent:
                # for dep in coreComponent['dependency']:
                    # if Name == "fs":
                        # for media_idx in range(1, 4):
                            # if (media_idx == 1):
                                # Component.addDependency("sys_" + Name + "_" + dep + str(media_idx) + "_dependency", dep, False, True)
                            # else:
                                # Component.addDependency("sys_" + Name + "_" + dep + str(media_idx) + "_dependency", dep, False, False)
                    # else:
                        # Component.addDependency("sys_" + Name + "_" + dep + "_dependency", dep, False, True)

            # Component.setDisplayType("System Service")
        # #create driver component
        # else:
            # print("create component: " + Name.upper() + " Driver")

            # if coreComponent['instance'] == "multi":
                # Component = Module.CreateGeneratorComponent("drv_" + Name, Label, "/Harmony/Drivers/", "driver/" + Name + "/config/drv_" + Name + "_common.py", "driver/" + Name + "/config/drv_" + Name + ".py")
            # elif coreComponent['instance'] == "single":
                # Component = Module.CreateComponent("drv_" + Name, Label, "/Harmony/Drivers/", "driver/" + Name + "/config/drv_" + Name + ".py")

            # if "capability" in coreComponent:
                # for capability in coreComponent['capability']:
                    # Component.addCapability(capability.lower(), capability)

            # Component.setDisplayType("Driver")
            # if "dependency" in coreComponent:
                # for dep in coreComponent['dependency']:
                    # Component.addDependency("drv_" + Name + "_" + dep + "_dependency", dep, False, True)
					
	print("create component: USB Device CDC")
	usbDeviceCdcComponent = Module.CreateGeneratorComponent("usb_device_cdc", "CDC Function Driver", "/Libraries/USB/Device Stack", "config/usb_device_cdc_common.py", "config/usb_device_cdc.py")
	usbDeviceCdcComponent.addDependency("usb_device_dependency", "USB_DEVICE", True, True)
	usbDeviceCdcComponent.addCapability("USB Device", "USB_DEVICE_CDC")
	
	print("create component: USB Device Vendor")
	usbDeviceVendorComponent = Module.CreateGeneratorComponent("usb_device_vendor", "Vendor Function", "/Libraries/USB/Device Stack", "config/usb_device_vendor_common.py", "config/usb_device_vendor.py")
	usbDeviceVendorComponent.addDependency("usb_device_dependency", "USB_DEVICE" , True, True)
	usbDeviceVendorComponent.addCapability("USB Device", "USB_DEVICE_VENDOR")
	
	print("create component: USB Device Audio")
	usbDeviceAudioComponent = Module.CreateGeneratorComponent("usb_device_audio", "Audio Function Driver", "/Libraries/USB/Device Stack", "config/usb_device_audio_common.py", "config/usb_device_audio.py")
	usbDeviceAudioComponent.addDependency("usb_device_dependency", "USB_DEVICE" , True, True)
	usbDeviceAudioComponent.addCapability("USB Device", "USB_DEVICE_AUDIO")
	
	print("create component: USB Device HID")
	usbDeviceHidComponent = Module.CreateGeneratorComponent("usb_device_hid", "HID Function Driver", "/Libraries/USB/Device Stack", "config/usb_device_hid_common.py", "config/usb_device_hid.py")
	usbDeviceHidComponent.addDependency("usb_device_dependency", "USB_DEVICE" , True, True)
	usbDeviceHidComponent.addCapability("USB Device", "USB_DEVICE_HID")
	
	print("create component: USB Device MSD")
	usbDeviceMsdComponent = Module.CreateGeneratorComponent("usb_device_msd", "MSD Function Driver", "/Libraries/USB/Device Stack", "config/usb_device_msd_common.py", "config/usb_device_msd.py")
	usbDeviceMsdComponent.addDependency("usb_device_dependency", "USB_DEVICE", True, True)
	usbDeviceMsdComponent.addDependency("usb_device_msd_meida_dependency_1", "DRV_MEDIA")
	usbDeviceMsdComponent.addDependency("usb_device_msd_meida_dependency_2", "DRV_MEDIA")
	usbDeviceMsdComponent.addDependency("usb_device_msd_meida_dependency_3", "DRV_MEDIA")
	#usbDeviceMsdComponent.addDependency("usb_device_meida_dependency_4", "DRV_MEDIA")
	usbDeviceMsdComponent.addCapability("USB Device", "USB_DEVICE_MSD")
	
	
	print("create component: USB Host")
	usbHostComponent = Module.CreateSharedComponent("usb_host", "Host Layer", "/Libraries/USB/Host Stack", "config/usb_host.py")
	usbHostComponent.addDependency("usb_driver_dependency", "DRV_USB")
	usbHostComponent.addDependency("usb_host_tmr_dependency", "SYS_TIME")
	usbHostComponent.addCapability("usb_host", "USB_HOST")
	
	print("create component: USB Host MSD")
	usbHostMsdComponent = Module.CreateComponent("usb_host_msd", "MSD Client Driver", "/Libraries/USB/Host Stack","config/usb_host_msd.py")
	usbHostMsdComponent.addDependency("usb_host_dependency", "USB_HOST")
	
	print("create component: USB Host CDC")
	usbHostCdcComponent = Module.CreateComponent("usb_host_cdc", "CDC Client Driver", "/Libraries/USB/Host Stack", "config/usb_host_cdc.py")
	usbHostCdcComponent.addDependency("usb_host_dependency", "USB_HOST")
	
	print("create component: USB Host HID")
	usbHostHidComponent = Module.CreateComponent("usb_host_hid", "HID Client Driver", "/Libraries/USB/Host Stack", "config/usb_host_hid.py")
	usbHostHidComponent.addDependency("usb_host_dependency", "USB_HOST")

	print("create component: USB Host Audio")
	usbHostAudioComponent = Module.CreateComponent("usb_host_audio", "Audio Client Driver", "/Libraries/USB/Host Stack", "config/usb_host_audio.py")
	usbHostAudioComponent.addDependency("usb_host_dependency", "USB_HOST")
		