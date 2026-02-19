extern "C" {

void sceKernelRegisterSubIntrHandler(void) {
}

void sceKernelReleaseSubIntrHandler(void) {
}

void sceKernelEnableSubIntr(void) {
}

void sceIoWrite(void) {
}

void sceIoDevctl(void) {
}

void sceIoRead(void) {
}

void sceIoRename(void) {
}

void sceIoClose(void) {
}

void sceIoGetstat(void) {
}

void sceIoDopen(void) {
}

void sceIoDclose(void) {
}

void sceIoOpen(void) {
}

void sceIoLseek(void) {
}

void sceKernelCpuSuspendIntr(void) {
}

void sceKernelCpuResumeIntr(void) {
}

void sceKernelExitGame(void) {
}

void sceKernelRegisterExitCallback(void) {
}

void sceKernelUnloadModule(void) {
}

void sceKernelGetModuleIdByAddress(void) {
}

void sceKernelGetModuleId(void) {
}

void sceKernelStartModule(void) {
}

void sceKernelStopUnloadSelfModuleWithStatus(void) {
}

void sceKernelLoadModuleByID(void) {
}

void sceKernelStopModule(void) {
}

void sceKernelStdin(void) {
}

void sceKernelStdout(void) {
}

void sceKernelStderr(void) {
}

void sceKernelPrintf(void) {
}

void sceKernelAllocPartitionMemory(void) {
}

void sceKernelSetCompiledSdkVersion370(void) {
}

void sceKernelGetBlockHeadAddr(void) {
}

void sceKernelFreePartitionMemory(void) {
}

void sceKernelSetCompilerVersion(void) {
}

void sceKernelStartVTimer(void) {
}

void sceKernelDelayThread(void) {
}

void sceKernelWakeupThread(void) {
}

void sceKernelSetEventFlag(void) {
}

void sceKernelCreateSema(void) {
}

void sceKernelSysClock2USecWide(void) {
}

void sceKernelCreateCallback(void) {
}

void sceKernelCreateVTimer(void) {
}

void sceKernelChangeCurrentThreadAttr(void) {
}

void sceKernelDeleteEventFlag(void) {
}

void sceKernelStartThread(void) {
}

void sceKernelWaitThreadEnd(void) {
}

void sceKernelResumeDispatchThread(void) {
}

void sceKernelDeleteSema(void) {
}

void sceKernelGetThreadId(void) {
}

void sceKernelPollEventFlag(void) {
}

void sceKernelGetSystemTimeLow(void) {
}

void sceKernelTerminateDeleteThread(void) {
}

void sceKernelSuspendDispatchThread(void) {
}

void sceKernelGetThreadExitStatus(void) {
}

void sceKernelSignalSema(void) {
}

void sceKernelWaitEventFlag(void) {
}

void sceKernelCreateThread(void) {
}

void sceKernelWaitSema(void) {
}

void sceKernelSetVTimerHandlerWide(void) {
}

void sceKernelCreateEventFlag(void) {
}

void sceKernelPollSema(void) {
}

void sceKernelTerminateThread(void) {
}

void sceKernelDelayThreadCB(void) {
}

void sceKernelChangeThreadPriority(void) {
}

void sceKernelExitDeleteThread(void) {
}

void sceKernelClearEventFlag(void) {
}

void sceKernelSleepThreadCB(void) {
}

void sceKernelGetSystemTimeWide(void) {
}

void sceKernelGetThreadCurrentPriority(void) {
}

void sceKernelSleepThread(void) {
}

void sceKernelDeleteThread(void) {
}

void sceKernelExitThread(void) {
}

void sceKernelLibcGettimeofday(void) {
}

void sceKernelDcacheWritebackAll(void) {
}

void sceKernelLibcClock(void) {
}

void sceKernelIcacheInvalidateAll(void) {
}

void sceKernelDcacheWritebackInvalidateAll(void) {
}

void sceKernelDcacheInvalidateRange(void) {
}

void sceKernelLibcTime(void) {
}

void sceKernelDcacheWritebackRange(void) {
}

void sceAtracGetStreamDataInfo(void) {
}

void sceAtracReleaseAtracID(void) {
}

void sceAtracDecodeData(void) {
}

void sceAtracSetDataAndGetID(void) {
}

void sceAtracSetLoopNum(void) {
}

void sceAtracGetRemainFrame(void) {
}

void sceAtracGetSoundSample(void) {
}

void sceAtracGetBitrate(void) {
}

void sceAtracGetNextDecodePosition(void) {
}

void sceAtracGetLoopStatus(void) {
}

void sceAudioOutputBlocking(void) {
}

void sceAudioOutputPannedBlocking(void) {
}

void sceAudioChReserve(void) {
}

void sceAudioChRelease(void) {
}

void sceAudioChangeChannelConfig(void) {
}

void sceAudioGetChannelRestLength(void) {
}

void sceAudioChangeChannelVolume(void) {
}

void sceAudioSetChannelDataLen(void) {
}

void sceAudioOutputPanned(void) {
}

void sceCtrlSetSamplingMode(void) {
}

void sceCtrlReadBufferPositive(void) {
}

void sceCtrlSetSamplingCycle(void) {
}

void sceDisplaySetMode(void) {
}

void sceDisplaySetFrameBuf(void) {
}

void sceDmacMemcpy(void) {
}

void sceGeListSync(void) {
}

void sceGeUnsetCallback(void) {
}

void sceGeListEnQueueHead(void) {
}

void sceGeEdramGetSize(void) {
}

void sceGeContinue(void) {
}

void sceGeSetCallback(void) {
}

void sceGeListEnQueue(void) {
}

void sceGeDrawSync(void) {
}

void sceGeBreak(void) {
}

void sceGeEdramSetAddrTranslation(void) {
}

void sceGeListUpdateStallAddr(void) {
}

void sceGeEdramGetAddr(void) {
}

void sceHttpGetContentLength(void) {
}

void sceHttpDisableCookie(void) {
}

void sceHttpSetRecvTimeOut(void) {
}

void sceHttpGetNetworkPspError(void) {
}

void sceHttpAddExtraHeader(void) {
}

void sceHttpGetStatusCode(void) {
}

void sceHttpDeleteConnection(void) {
}

void sceHttpsLoadDefaultCert(void) {
}

void sceHttpSetConnectTimeOut(void) {
}

void sceHttpSetSendTimeOut(void) {
}

void sceHttpCreateTemplate(void) {
}

void sceHttpSetRedirectCallback(void) {
}

void sceHttpDeleteRequest(void) {
}

void sceHttpsGetSslError(void) {
}

void sceHttpInit(void) {
}

void sceHttpDisableAuth(void) {
}

void sceHttpCreateRequestWithURL(void) {
}

void sceHttpSendRequest(void) {
}

void sceHttpAbortRequest(void) {
}

void sceHttpDisableCache(void) {
}

void sceHttpCreateConnectionWithURL(void) {
}

void sceHttpEnd(void) {
}

void sceHttpGetAllHeader(void) {
}

void sceHttpsInit(void) {
}

void sceHttpReadData(void) {
}

void sceHttpSetMallocFunction(void) {
}

void sceHttpsEnd(void) {
}

void sceHttpDeleteTemplate(void) {
}

void sceFontFindOptimumFont(void) {
}

void sceFontGetFontInfo(void) {
}

void sceFontGetNumFontList(void) {
}

void sceFontClose(void) {
}

void sceFontDoneLib(void) {
}

void sceFontNewLib(void) {
}

void sceFontGetCharGlyphImage(void) {
}

void sceFontOpen(void) {
}

void sceFontGetCharInfo(void) {
}

void sceMpegAvcDecode(void) {
}

void sceMpegRingbufferDestruct(void) {
}

void sceMpegInitAu(void) {
}

void sceMpegQueryStreamOffset(void) {
}

void sceMpegRingbufferConstruct(void) {
}

void sceMpegRegistStream(void) {
}

void sceMpegUnRegistStream(void) {
}

void sceMpegDelete(void) {
}

void sceMpegQueryStreamSize(void) {
}

void sceMpegInit(void) {
}

void sceMpegFlushAllStream(void) {
}

void sceMpegAvcDecodeStop(void) {
}

void sceMpegAtracDecode(void) {
}

void sceMpegFinish(void) {
}

void sceMpegMallocAvcEsBuf(void) {
}

void sceMpegRingbufferPut(void) {
}

void sceMpegRingbufferAvailableSize(void) {
}

void sceMpegQueryMemSize(void) {
}

void sceMpegFreeAvcEsBuf(void) {
}

void sceMpegRingbufferQueryMemSize(void) {
}

void sceMpegCreate(void) {
}

void sceMpegGetAtracAu(void) {
}

void sceMpegQueryAtracEsSize(void) {
}

void sceMpegGetAvcAu(void) {
}

void sceNetAdhocPtpClose(void) {
}

void sceNetAdhocPtpSend(void) {
}

void sceNetAdhocPdpCreate(void) {
}

void sceNetAdhocPdpDelete(void) {
}

void sceNetAdhocPtpOpen(void) {
}

void sceNetAdhocPtpRecv(void) {
}

void sceNetAdhocPtpFlush(void) {
}

void sceNetAdhocPtpAccept(void) {
}

void sceNetAdhocTerm(void) {
}

void sceNetAdhocPdpSend(void) {
}

void sceNetAdhocGetPtpStat(void) {
}

void sceNetAdhocPdpRecv(void) {
}

void sceNetAdhocPtpListen(void) {
}

void sceNetAdhocInit(void) {
}

void sceNetAdhocPtpConnect(void) {
}

void sceNetAdhocDiscoverUpdate(void) {
}

void sceNetAdhocDiscoverInitStart(void) {
}

void sceNetAdhocDiscoverGetStatus(void) {
}

void sceNetAdhocDiscoverTerm(void) {
}

void sceNetAdhocDiscoverRequestSuspend(void) {
}

void sceNetAdhocctlScan(void) {
}

void sceNetAdhocctlAddHandler(void) {
}

void sceNetAdhocctlDisconnect(void) {
}

void sceNetAdhocctlDelHandler(void) {
}

void sceNetAdhocctlGetScanInfo(void) {
}

void sceNetAdhocctlTerm(void) {
}

void sceNetAdhocctlGetPeerList(void) {
}

void sceNetAdhocctlInit(void) {
}

void sceNetApctlDisconnect(void) {
}

void sceNetApctlGetInfo(void) {
}

void sceNetApctlDelHandler(void) {
}

void sceNetApctlAddHandler(void) {
}

void sceNetApctlTerm(void) {
}

void sceNetApctlInit(void) {
}

void sceNetInetInit(void) {
}

void sceNetInetBind(void) {
}

void sceNetInetSetsockopt(void) {
}

void sceNetInetConnect(void) {
}

void sceNetInetSend(void) {
}

void sceNetInetCloseWithRST(void) {
}

void sceNetInetSocket(void) {
}

void sceNetInetGetPspError(void) {
}

void sceNetInetClose(void) {
}

void sceNetInetTerm(void) {
}

void sceNetInetGetTcpcbstat(void) {
}

void sceNetInetInetAddr(void) {
}

void sceNetInetRecv(void) {
}

void sceNetInetListen(void) {
}

void sceNetInetAccept(void) {
}

void sceNetInetGetpeername(void) {
}

void sceNetGetLocalEtherAddr(void) {
}

void sceNetTerm(void) {
}

void sceNetInit(void) {
}

void sceNetFreeThreadinfo(void) {
}

void sceOpenPSIDGetOpenPSID(void) {
}

void scePowerRegisterCallback(void) {
}

void sceRtcGetCurrentClockLocalTime(void) {
}

void sceSasSetADSR(void) {
}

void sceSasGetAllEnvelopeHeights(void) {
}

void sceSasRevParam(void) {
}

void sceSasGetPauseFlag(void) {
}

void sceSasRevType(void) {
}

void sceSasInit(void) {
}

void sceSasSetVolume(void) {
}

void sceSasCoreWithMix(void) {
}

void sceSasSetSL(void) {
}

void sceSasGetEndFlag(void) {
}

void sceSasGetEnvelopeHeight(void) {
}

void sceSasSetKeyOn(void) {
}

void sceSasSetPause(void) {
}

void sceSasSetVoice(void) {
}

void sceSasSetADSRmode(void) {
}

void sceSasSetKeyOff(void) {
}

void sceSasCore(void) {
}

void sceSasSetPitch(void) {
}

void sceSasSetNoise(void) {
}

void sceSasGetGrain(void) {
}

void sceSasSetSimpleADSR(void) {
}

void sceSasSetGrain(void) {
}

void sceSasRevEVOL(void) {
}

void sceSasGetOutputmode(void) {
}

void sceSasSetOutputmode(void) {
}

void sceSasRevVON(void) {
}

void sceKernelPowerTick(void) {
}

void sceKernelVolatileMemLock(void) {
}

void sceKernelVolatileMemUnlock(void) {
}

void sceUmdGetErrorStat(void) {
}

void sceUmdGetDriveStat(void) {
}

void sceUmdActivate(void) {
}

void sceUmdDeactivate(void) {
}

void sceUtilityLoadModule(void) {
}

void sceUtilityMsgDialogInitStart(void) {
}

void sceUtilityOskShutdownStart(void) {
}

void sceUtilityOskUpdate(void) {
}

void sceUtilityNetconfInitStart(void) {
}

void sceUtilitySavedataInitStart(void) {
}

void sceUtilityNetconfGetStatus(void) {
}

void sceUtilityMsgDialogShutdownStart(void) {
}

void sceUtilitySavedataGetStatus(void) {
}

void sceUtilityNetconfUpdate(void) {
}

void sceUtilityMsgDialogUpdate(void) {
}

void sceUtilitySavedataShutdownStart(void) {
}

void sceUtilityMsgDialogGetStatus(void) {
}

void sceUtilityGetSystemParamInt(void) {
}

void sceUtilitySavedataUpdate(void) {
}

void sceUtilityUnloadModule(void) {
}

void sceUtilityOskGetStatus(void) {
}

void sceUtilityOskInitStart(void) {
}

void sceUtilityNetconfShutdownStart(void) {
}

void sceWlanGetEtherAddr(void) {
}

void sceWlanGetSwitchState(void) {
}

void sceSslEnd(void) {
}

void sceSslInit(void) {
}

}