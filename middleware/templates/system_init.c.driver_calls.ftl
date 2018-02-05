<#if (CONFIG_DRV_USB_DEVICE_SUPPORT == true) || (CONFIG_DRV_USB_HOST_SUPPORT == true)>
    <#if CONFIG_PIC32MZ == true >
    /* Initialize USB Driver */ 
    sysObj.drvUSBObject = DRV_USBHS_Initialize(DRV_USBHS_INDEX_0, (SYS_MODULE_INIT *) &drvUSBHSInit);
    
        <#if CONFIG_USE_SYS_INT == true &&  CONFIG_DRV_USB_INTERRUPT_MODE == true>
    /* Set priority of USB interrupt source */
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USB_INTERRUPT_VECTOR_IDX0}, ${CONFIG_DRV_USB_INTERRUPT_PRIORITY_IDX0});

    /* Set Sub-priority of USB interrupt source */
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USB_INTERRUPT_VECTOR_IDX0}, ${CONFIG_DRV_USB_INTERRUPT_SUB_PRIORITY_IDX0});
    
    /* Set the priority of the USB DMA Interrupt */
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USBDMA_INTERRUPT_VECTOR_IDX0}, ${CONFIG_DRV_USBDMA_INTERRUPT_PRIORITY_IDX0});

    /* Set Sub-priority of the USB DMA Interrupt */
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USBDMA_INTERRUPT_VECTOR_IDX0}, ${CONFIG_DRV_USBDMA_INTERRUPT_SUB_PRIORITY_IDX0});
        </#if>
    
    <#elseif ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true) || (CONFIG_PIC32WK == true))>
    /* Initialize USB Driver */ 
    sysObj.drvUSBObject = DRV_USBFS_Initialize(DRV_USBFS_INDEX_0, (SYS_MODULE_INIT *) &drvUSBFSInit);
    
        <#if CONFIG_USE_SYS_INT == true &&  CONFIG_DRV_USB_INTERRUPT_MODE == true>
    /* Set priority of USB interrupt source */
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USB_INTERRUPT_VECTOR_IDX0}, ${CONFIG_DRV_USB_INTERRUPT_PRIORITY_IDX0});

    /* Set Sub-priority of USB interrupt source */
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USB_INTERRUPT_VECTOR_IDX0}, ${CONFIG_DRV_USB_INTERRUPT_SUB_PRIORITY_IDX0});
        </#if>
    </#if>
</#if>
