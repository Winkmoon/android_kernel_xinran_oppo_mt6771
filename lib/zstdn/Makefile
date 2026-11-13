# SPDX-License-Identifier: GPL-2.0+ OR BSD-3-Clause
#
# OPPO's vendored "zstdn" (newer zstd).  Its symbols are prefixed because
# the in-kernel zstd (used by f2fs) exports the same ZSTD_/HUF_/FSE_/xxhash
# helpers.

ccflags-y += -I$(srctree)/lib/zstdn/include
# the vendored sources assume size_t and the kernel types are visible
ccflags-y += -include linux/types.h

ccflags-y += -DERR_getErrorString=zstdn_ERR_getErrorString \
	 -DFSE_buildDTable_raw=zstdn_FSE_buildDTable_raw -DFSE_buildDTable_rle=zstdn_FSE_buildDTable_rle \
	 -DFSE_buildDTable_wksp=zstdn_FSE_buildDTable_wksp -DFSE_createCTable=zstdn_FSE_createCTable \
	 -DFSE_createDTable=zstdn_FSE_createDTable \
	 -DFSE_decompress_usingDTable=zstdn_FSE_decompress_usingDTable \
	 -DFSE_decompress_wksp=zstdn_FSE_decompress_wksp \
	 -DFSE_decompress_wksp_bmi2=zstdn_FSE_decompress_wksp_bmi2 -DFSE_freeCTable=zstdn_FSE_freeCTable \
	 -DFSE_freeDTable=zstdn_FSE_freeDTable -DFSE_getErrorName=zstdn_FSE_getErrorName \
	 -DFSE_readNCount=zstdn_FSE_readNCount \
	 -DFSE_readNCount_bmi2=zstdn_FSE_readNCount_bmi2 -DFSE_versionNumber=zstdn_FSE_versionNumber \
	 -DHUF_buildCTable_wksp=zstdn_HUF_buildCTable_wksp \
	 -DHUF_compress1X_repeat=zstdn_HUF_compress1X_repeat \
	 -DHUF_compress1X_usingCTable=zstdn_HUF_compress1X_usingCTable \
	 -DHUF_compress1X_wksp=zstdn_HUF_compress1X_wksp \
	 -DHUF_compress4X_repeat=zstdn_HUF_compress4X_repeat \
	 -DHUF_compress4X_usingCTable=zstdn_HUF_compress4X_usingCTable \
	 -DHUF_compress4X_wksp=zstdn_HUF_compress4X_wksp -DHUF_compressBound=zstdn_HUF_compressBound \
	 -DHUF_decompress1X1_DCtx_wksp=zstdn_HUF_decompress1X1_DCtx_wksp \
	 -DHUF_decompress1X1_DCtx_wksp_bmi2=zstdn_HUF_decompress1X1_DCtx_wksp_bmi2 \
	 -DHUF_decompress1X1_usingDTable=zstdn_HUF_decompress1X1_usingDTable \
	 -DHUF_decompress1X2_DCtx_wksp=zstdn_HUF_decompress1X2_DCtx_wksp \
	 -DHUF_decompress1X2_usingDTable=zstdn_HUF_decompress1X2_usingDTable \
	 -DHUF_decompress1X_DCtx_wksp=zstdn_HUF_decompress1X_DCtx_wksp \
	 -DHUF_decompress1X_usingDTable=zstdn_HUF_decompress1X_usingDTable \
	 -DHUF_decompress1X_usingDTable_bmi2=zstdn_HUF_decompress1X_usingDTable_bmi2 \
	 -DHUF_decompress4X1_DCtx_wksp=zstdn_HUF_decompress4X1_DCtx_wksp \
	 -DHUF_decompress4X1_usingDTable=zstdn_HUF_decompress4X1_usingDTable \
	 -DHUF_decompress4X2_DCtx_wksp=zstdn_HUF_decompress4X2_DCtx_wksp \
	 -DHUF_decompress4X2_usingDTable=zstdn_HUF_decompress4X2_usingDTable \
	 -DHUF_decompress4X_hufOnly_wksp=zstdn_HUF_decompress4X_hufOnly_wksp \
	 -DHUF_decompress4X_hufOnly_wksp_bmi2=zstdn_HUF_decompress4X_hufOnly_wksp_bmi2 \
	 -DHUF_decompress4X_usingDTable=zstdn_HUF_decompress4X_usingDTable \
	 -DHUF_decompress4X_usingDTable_bmi2=zstdn_HUF_decompress4X_usingDTable_bmi2 \
	 -DHUF_estimateCompressedSize=zstdn_HUF_estimateCompressedSize \
	 -DHUF_getErrorName=zstdn_HUF_getErrorName -DHUF_getNbBits=zstdn_HUF_getNbBits \
	 -DHUF_optimalTableLog=zstdn_HUF_optimalTableLog \
	 -DHUF_readCTable=zstdn_HUF_readCTable -DHUF_readDTableX1_wksp=zstdn_HUF_readDTableX1_wksp \
	 -DHUF_readDTableX1_wksp_bmi2=zstdn_HUF_readDTableX1_wksp_bmi2 \
	 -DHUF_readDTableX2_wksp=zstdn_HUF_readDTableX2_wksp -DHUF_readStats=zstdn_HUF_readStats \
	 -DHUF_readStats_wksp=zstdn_HUF_readStats_wksp -DHUF_selectDecoder=zstdn_HUF_selectDecoder \
	 -DHUF_validateCTable=zstdn_HUF_validateCTable -DHUF_writeCTable=zstdn_HUF_writeCTable \
	 -DHUF_writeCTable_wksp=zstdn_HUF_writeCTable_wksp \
	 -DZSTD_CCtxParams_getParameter=zstdn_ZSTD_CCtxParams_getParameter \
	 -DZSTD_CCtxParams_init=zstdn_ZSTD_CCtxParams_init \
	 -DZSTD_CCtxParams_init_advanced=zstdn_ZSTD_CCtxParams_init_advanced \
	 -DZSTD_CCtxParams_reset=zstdn_ZSTD_CCtxParams_reset \
	 -DZSTD_CCtxParams_setParameter=zstdn_ZSTD_CCtxParams_setParameter \
	 -DZSTD_CCtx_getParameter=zstdn_ZSTD_CCtx_getParameter \
	 -DZSTD_CCtx_loadDictionary=zstdn_ZSTD_CCtx_loadDictionary \
	 -DZSTD_CCtx_loadDictionary_advanced=zstdn_ZSTD_CCtx_loadDictionary_advanced \
	 -DZSTD_CCtx_loadDictionary_byReference=zstdn_ZSTD_CCtx_loadDictionary_byReference \
	 -DZSTD_CCtx_refCDict=zstdn_ZSTD_CCtx_refCDict -DZSTD_CCtx_refPrefix=zstdn_ZSTD_CCtx_refPrefix \
	 -DZSTD_CCtx_refPrefix_advanced=zstdn_ZSTD_CCtx_refPrefix_advanced \
	 -DZSTD_CCtx_refThreadPool=zstdn_ZSTD_CCtx_refThreadPool -DZSTD_CCtx_reset=zstdn_ZSTD_CCtx_reset \
	 -DZSTD_CCtx_setParameter=zstdn_ZSTD_CCtx_setParameter \
	 -DZSTD_CCtx_setParametersUsingCCtxParams=zstdn_ZSTD_CCtx_setParametersUsingCCtxParams \
	 -DZSTD_CCtx_setPledgedSrcSize=zstdn_ZSTD_CCtx_setPledgedSrcSize \
	 -DZSTD_CCtx_trace=zstdn_ZSTD_CCtx_trace -DZSTD_CStreamInSize=zstdn_ZSTD_CStreamInSize \
	 -DZSTD_CStreamOutSize=zstdn_ZSTD_CStreamOutSize \
	 -DZSTD_DCtx_getParameter=zstdn_ZSTD_DCtx_getParameter \
	 -DZSTD_DCtx_loadDictionary=zstdn_ZSTD_DCtx_loadDictionary \
	 -DZSTD_DCtx_loadDictionary_advanced=zstdn_ZSTD_DCtx_loadDictionary_advanced \
	 -DZSTD_DCtx_loadDictionary_byReference=zstdn_ZSTD_DCtx_loadDictionary_byReference \
	 -DZSTD_DCtx_refDDict=zstdn_ZSTD_DCtx_refDDict -DZSTD_DCtx_refPrefix=zstdn_ZSTD_DCtx_refPrefix \
	 -DZSTD_DCtx_refPrefix_advanced=zstdn_ZSTD_DCtx_refPrefix_advanced \
	 -DZSTD_DCtx_reset=zstdn_ZSTD_DCtx_reset -DZSTD_DCtx_setFormat=zstdn_ZSTD_DCtx_setFormat \
	 -DZSTD_DCtx_setMaxWindowSize=zstdn_ZSTD_DCtx_setMaxWindowSize \
	 -DZSTD_DCtx_setParameter=zstdn_ZSTD_DCtx_setParameter \
	 -DZSTD_DDict_dictContent=zstdn_ZSTD_DDict_dictContent \
	 -DZSTD_DDict_dictSize=zstdn_ZSTD_DDict_dictSize -DZSTD_DStreamInSize=zstdn_ZSTD_DStreamInSize \
	 -DZSTD_DStreamOutSize=zstdn_ZSTD_DStreamOutSize -DZSTD_adjustCParams=zstdn_ZSTD_adjustCParams \
	 -DZSTD_buildCTable=zstdn_ZSTD_buildCTable -DZSTD_buildFSETable=zstdn_ZSTD_buildFSETable \
	 -DZSTD_cParam_getBounds=zstdn_ZSTD_cParam_getBounds -DZSTD_checkCParams=zstdn_ZSTD_checkCParams \
	 -DZSTD_checkContinuity=zstdn_ZSTD_checkContinuity -DZSTD_compress=zstdn_ZSTD_compress \
	 -DZSTD_compress2=zstdn_ZSTD_compress2 -DZSTD_compressBegin=zstdn_ZSTD_compressBegin \
	 -DZSTD_compressBegin_advanced=zstdn_ZSTD_compressBegin_advanced \
	 -DZSTD_compressBegin_advanced_internal=zstdn_ZSTD_compressBegin_advanced_internal \
	 -DZSTD_compressBegin_usingCDict=zstdn_ZSTD_compressBegin_usingCDict \
	 -DZSTD_compressBegin_usingCDict_advanced=zstdn_ZSTD_compressBegin_usingCDict_advanced \
	 -DZSTD_compressBegin_usingDict=zstdn_ZSTD_compressBegin_usingDict \
	 -DZSTD_compressBlock=zstdn_ZSTD_compressBlock \
	 -DZSTD_compressBlock_btlazy2=zstdn_ZSTD_compressBlock_btlazy2 \
	 -DZSTD_compressBlock_btlazy2_dictMatchState=zstdn_ZSTD_compressBlock_btlazy2_dictMatchState \
	 -DZSTD_compressBlock_btlazy2_extDict=zstdn_ZSTD_compressBlock_btlazy2_extDict \
	 -DZSTD_compressBlock_btopt=zstdn_ZSTD_compressBlock_btopt \
	 -DZSTD_compressBlock_btopt_dictMatchState=zstdn_ZSTD_compressBlock_btopt_dictMatchState \
	 -DZSTD_compressBlock_btopt_extDict=zstdn_ZSTD_compressBlock_btopt_extDict \
	 -DZSTD_compressBlock_btultra=zstdn_ZSTD_compressBlock_btultra \
	 -DZSTD_compressBlock_btultra2=zstdn_ZSTD_compressBlock_btultra2 \
	 -DZSTD_compressBlock_btultra_dictMatchState=zstdn_ZSTD_compressBlock_btultra_dictMatchState \
	 -DZSTD_compressBlock_btultra_extDict=zstdn_ZSTD_compressBlock_btultra_extDict \
	 -DZSTD_compressBlock_doubleFast=zstdn_ZSTD_compressBlock_doubleFast \
	 -DZSTD_compressBlock_doubleFast_dictMatchState=zstdn_ZSTD_compressBlock_doubleFast_dictMatchState \
	 -DZSTD_compressBlock_doubleFast_extDict=zstdn_ZSTD_compressBlock_doubleFast_extDict \
	 -DZSTD_compressBlock_fast=zstdn_ZSTD_compressBlock_fast \
	 -DZSTD_compressBlock_fast_dictMatchState=zstdn_ZSTD_compressBlock_fast_dictMatchState \
	 -DZSTD_compressBlock_fast_extDict=zstdn_ZSTD_compressBlock_fast_extDict \
	 -DZSTD_compressBlock_greedy=zstdn_ZSTD_compressBlock_greedy \
	 -DZSTD_compressBlock_greedy_dedicatedDictSearch=zstdn_ZSTD_compressBlock_greedy_dedicatedDictSearch \
	 -DZSTD_compressBlock_greedy_dictMatchState=zstdn_ZSTD_compressBlock_greedy_dictMatchState \
	 -DZSTD_compressBlock_greedy_extDict=zstdn_ZSTD_compressBlock_greedy_extDict \
	 -DZSTD_compressBlock_lazy=zstdn_ZSTD_compressBlock_lazy \
	 -DZSTD_compressBlock_lazy2=zstdn_ZSTD_compressBlock_lazy2 \
	 -DZSTD_compressBlock_lazy2_dedicatedDictSearch=zstdn_ZSTD_compressBlock_lazy2_dedicatedDictSearch \
	 -DZSTD_compressBlock_lazy2_dictMatchState=zstdn_ZSTD_compressBlock_lazy2_dictMatchState \
	 -DZSTD_compressBlock_lazy2_extDict=zstdn_ZSTD_compressBlock_lazy2_extDict \
	 -DZSTD_compressBlock_lazy_dedicatedDictSearch=zstdn_ZSTD_compressBlock_lazy_dedicatedDictSearch \
	 -DZSTD_compressBlock_lazy_dictMatchState=zstdn_ZSTD_compressBlock_lazy_dictMatchState \
	 -DZSTD_compressBlock_lazy_extDict=zstdn_ZSTD_compressBlock_lazy_extDict \
	 -DZSTD_compressBound=zstdn_ZSTD_compressBound -DZSTD_compressCCtx=zstdn_ZSTD_compressCCtx \
	 -DZSTD_compressContinue=zstdn_ZSTD_compressContinue -DZSTD_compressEnd=zstdn_ZSTD_compressEnd \
	 -DZSTD_compressLiterals=zstdn_ZSTD_compressLiterals \
	 -DZSTD_compressRleLiteralsBlock=zstdn_ZSTD_compressRleLiteralsBlock \
	 -DZSTD_compressSequences=zstdn_ZSTD_compressSequences \
	 -DZSTD_compressStream=zstdn_ZSTD_compressStream -DZSTD_compressStream2=zstdn_ZSTD_compressStream2 \
	 -DZSTD_compressStream2_simpleArgs=zstdn_ZSTD_compressStream2_simpleArgs \
	 -DZSTD_compressSuperBlock=zstdn_ZSTD_compressSuperBlock \
	 -DZSTD_compress_advanced=zstdn_ZSTD_compress_advanced \
	 -DZSTD_compress_advanced_internal=zstdn_ZSTD_compress_advanced_internal \
	 -DZSTD_compress_usingCDict=zstdn_ZSTD_compress_usingCDict \
	 -DZSTD_compress_usingCDict_advanced=zstdn_ZSTD_compress_usingCDict_advanced \
	 -DZSTD_compress_usingDict=zstdn_ZSTD_compress_usingDict -DZSTD_copyCCtx=zstdn_ZSTD_copyCCtx \
	 -DZSTD_copyDCtx=zstdn_ZSTD_copyDCtx -DZSTD_copyDDictParameters=zstdn_ZSTD_copyDDictParameters \
	 -DZSTD_createCCtx=zstdn_ZSTD_createCCtx -DZSTD_createCCtxParams=zstdn_ZSTD_createCCtxParams \
	 -DZSTD_createCCtx_advanced=zstdn_ZSTD_createCCtx_advanced \
	 -DZSTD_createCDict=zstdn_ZSTD_createCDict \
	 -DZSTD_createCDict_advanced=zstdn_ZSTD_createCDict_advanced \
	 -DZSTD_createCDict_advanced2=zstdn_ZSTD_createCDict_advanced2 \
	 -DZSTD_createCDict_byReference=zstdn_ZSTD_createCDict_byReference \
	 -DZSTD_createCStream=zstdn_ZSTD_createCStream \
	 -DZSTD_createCStream_advanced=zstdn_ZSTD_createCStream_advanced \
	 -DZSTD_createDCtx=zstdn_ZSTD_createDCtx -DZSTD_createDCtx_advanced=zstdn_ZSTD_createDCtx_advanced \
	 -DZSTD_createDDict=zstdn_ZSTD_createDDict \
	 -DZSTD_createDDict_advanced=zstdn_ZSTD_createDDict_advanced \
	 -DZSTD_createDDict_byReference=zstdn_ZSTD_createDDict_byReference \
	 -DZSTD_createDStream=zstdn_ZSTD_createDStream \
	 -DZSTD_createDStream_advanced=zstdn_ZSTD_createDStream_advanced \
	 -DZSTD_crossEntropyCost=zstdn_ZSTD_crossEntropyCost -DZSTD_customCalloc=zstdn_ZSTD_customCalloc \
	 -DZSTD_customFree=zstdn_ZSTD_customFree -DZSTD_customMalloc=zstdn_ZSTD_customMalloc \
	 -DZSTD_cycleLog=zstdn_ZSTD_cycleLog -DZSTD_dParam_getBounds=zstdn_ZSTD_dParam_getBounds \
	 -DZSTD_decodeLiteralsBlock=zstdn_ZSTD_decodeLiteralsBlock \
	 -DZSTD_decodeSeqHeaders=zstdn_ZSTD_decodeSeqHeaders \
	 -DZSTD_decodingBufferSize_min=zstdn_ZSTD_decodingBufferSize_min \
	 -DZSTD_decompress=zstdn_ZSTD_decompress -DZSTD_decompressBegin=zstdn_ZSTD_decompressBegin \
	 -DZSTD_decompressBegin_usingDDict=zstdn_ZSTD_decompressBegin_usingDDict \
	 -DZSTD_decompressBegin_usingDict=zstdn_ZSTD_decompressBegin_usingDict \
	 -DZSTD_decompressBlock=zstdn_ZSTD_decompressBlock \
	 -DZSTD_decompressBlock_internal=zstdn_ZSTD_decompressBlock_internal \
	 -DZSTD_decompressBound=zstdn_ZSTD_decompressBound \
	 -DZSTD_decompressContinue=zstdn_ZSTD_decompressContinue \
	 -DZSTD_decompressDCtx=zstdn_ZSTD_decompressDCtx \
	 -DZSTD_decompressStream=zstdn_ZSTD_decompressStream \
	 -DZSTD_decompressStream_simpleArgs=zstdn_ZSTD_decompressStream_simpleArgs \
	 -DZSTD_decompress_usingDDict=zstdn_ZSTD_decompress_usingDDict \
	 -DZSTD_decompress_usingDict=zstdn_ZSTD_decompress_usingDict \
	 -DZSTD_dedicatedDictSearch_lazy_loadDictionary=zstdn_ZSTD_dedicatedDictSearch_lazy_loadDictionary \
	 -DZSTD_encodeSequences=zstdn_ZSTD_encodeSequences -DZSTD_endStream=zstdn_ZSTD_endStream \
	 -DZSTD_estimateCCtxSize=zstdn_ZSTD_estimateCCtxSize \
	 -DZSTD_estimateCCtxSize_usingCCtxParams=zstdn_ZSTD_estimateCCtxSize_usingCCtxParams \
	 -DZSTD_estimateCCtxSize_usingCParams=zstdn_ZSTD_estimateCCtxSize_usingCParams \
	 -DZSTD_estimateCDictSize=zstdn_ZSTD_estimateCDictSize \
	 -DZSTD_estimateCDictSize_advanced=zstdn_ZSTD_estimateCDictSize_advanced \
	 -DZSTD_estimateCStreamSize=zstdn_ZSTD_estimateCStreamSize \
	 -DZSTD_estimateCStreamSize_usingCCtxParams=zstdn_ZSTD_estimateCStreamSize_usingCCtxParams \
	 -DZSTD_estimateCStreamSize_usingCParams=zstdn_ZSTD_estimateCStreamSize_usingCParams \
	 -DZSTD_estimateDCtxSize=zstdn_ZSTD_estimateDCtxSize \
	 -DZSTD_estimateDDictSize=zstdn_ZSTD_estimateDDictSize \
	 -DZSTD_estimateDStreamSize=zstdn_ZSTD_estimateDStreamSize \
	 -DZSTD_estimateDStreamSize_fromFrame=zstdn_ZSTD_estimateDStreamSize_fromFrame \
	 -DZSTD_fillDoubleHashTable=zstdn_ZSTD_fillDoubleHashTable \
	 -DZSTD_fillHashTable=zstdn_ZSTD_fillHashTable \
	 -DZSTD_findDecompressedSize=zstdn_ZSTD_findDecompressedSize \
	 -DZSTD_findFrameCompressedSize=zstdn_ZSTD_findFrameCompressedSize \
	 -DZSTD_flushStream=zstdn_ZSTD_flushStream -DZSTD_frameHeaderSize=zstdn_ZSTD_frameHeaderSize \
	 -DZSTD_freeCCtx=zstdn_ZSTD_freeCCtx -DZSTD_freeCCtxParams=zstdn_ZSTD_freeCCtxParams \
	 -DZSTD_freeCDict=zstdn_ZSTD_freeCDict -DZSTD_freeCStream=zstdn_ZSTD_freeCStream \
	 -DZSTD_freeDCtx=zstdn_ZSTD_freeDCtx -DZSTD_freeDDict=zstdn_ZSTD_freeDDict \
	 -DZSTD_freeDStream=zstdn_ZSTD_freeDStream -DZSTD_fseBitCost=zstdn_ZSTD_fseBitCost \
	 -DZSTD_generateSequences=zstdn_ZSTD_generateSequences -DZSTD_getBlockSize=zstdn_ZSTD_getBlockSize \
	 -DZSTD_getCParams=zstdn_ZSTD_getCParams \
	 -DZSTD_getCParamsFromCCtxParams=zstdn_ZSTD_getCParamsFromCCtxParams \
	 -DZSTD_getCParamsFromCDict=zstdn_ZSTD_getCParamsFromCDict \
	 -DZSTD_getDecompressedSize=zstdn_ZSTD_getDecompressedSize \
	 -DZSTD_getDictID_fromCDict=zstdn_ZSTD_getDictID_fromCDict \
	 -DZSTD_getDictID_fromDDict=zstdn_ZSTD_getDictID_fromDDict \
	 -DZSTD_getDictID_fromDict=zstdn_ZSTD_getDictID_fromDict \
	 -DZSTD_getDictID_fromFrame=zstdn_ZSTD_getDictID_fromFrame \
	 -DZSTD_getErrorCode=zstdn_ZSTD_getErrorCode -DZSTD_getErrorName=zstdn_ZSTD_getErrorName \
	 -DZSTD_getErrorString=zstdn_ZSTD_getErrorString \
	 -DZSTD_getFrameContentSize=zstdn_ZSTD_getFrameContentSize \
	 -DZSTD_getFrameHeader=zstdn_ZSTD_getFrameHeader \
	 -DZSTD_getFrameHeader_advanced=zstdn_ZSTD_getFrameHeader_advanced \
	 -DZSTD_getFrameProgression=zstdn_ZSTD_getFrameProgression -DZSTD_getParams=zstdn_ZSTD_getParams \
	 -DZSTD_getSeqStore=zstdn_ZSTD_getSeqStore -DZSTD_getcBlockSize=zstdn_ZSTD_getcBlockSize \
	 -DZSTD_initCStream=zstdn_ZSTD_initCStream \
	 -DZSTD_initCStream_advanced=zstdn_ZSTD_initCStream_advanced \
	 -DZSTD_initCStream_internal=zstdn_ZSTD_initCStream_internal \
	 -DZSTD_initCStream_srcSize=zstdn_ZSTD_initCStream_srcSize \
	 -DZSTD_initCStream_usingCDict=zstdn_ZSTD_initCStream_usingCDict \
	 -DZSTD_initCStream_usingCDict_advanced=zstdn_ZSTD_initCStream_usingCDict_advanced \
	 -DZSTD_initCStream_usingDict=zstdn_ZSTD_initCStream_usingDict \
	 -DZSTD_initDStream=zstdn_ZSTD_initDStream \
	 -DZSTD_initDStream_usingDDict=zstdn_ZSTD_initDStream_usingDDict \
	 -DZSTD_initDStream_usingDict=zstdn_ZSTD_initDStream_usingDict \
	 -DZSTD_initStaticCCtx=zstdn_ZSTD_initStaticCCtx -DZSTD_initStaticCDict=zstdn_ZSTD_initStaticCDict \
	 -DZSTD_initStaticCStream=zstdn_ZSTD_initStaticCStream \
	 -DZSTD_initStaticDCtx=zstdn_ZSTD_initStaticDCtx -DZSTD_initStaticDDict=zstdn_ZSTD_initStaticDDict \
	 -DZSTD_initStaticDStream=zstdn_ZSTD_initStaticDStream \
	 -DZSTD_insertAndFindFirstIndex=zstdn_ZSTD_insertAndFindFirstIndex \
	 -DZSTD_insertBlock=zstdn_ZSTD_insertBlock -DZSTD_invalidateRepCodes=zstdn_ZSTD_invalidateRepCodes \
	 -DZSTD_isFrame=zstdn_ZSTD_isFrame \
	 -DZSTD_ldm_adjustParameters=zstdn_ZSTD_ldm_adjustParameters \
	 -DZSTD_ldm_blockCompress=zstdn_ZSTD_ldm_blockCompress \
	 -DZSTD_ldm_fillHashTable=zstdn_ZSTD_ldm_fillHashTable \
	 -DZSTD_ldm_generateSequences=zstdn_ZSTD_ldm_generateSequences \
	 -DZSTD_ldm_getMaxNbSeq=zstdn_ZSTD_ldm_getMaxNbSeq \
	 -DZSTD_ldm_getTableSize=zstdn_ZSTD_ldm_getTableSize \
	 -DZSTD_ldm_skipRawSeqStoreBytes=zstdn_ZSTD_ldm_skipRawSeqStoreBytes \
	 -DZSTD_ldm_skipSequences=zstdn_ZSTD_ldm_skipSequences -DZSTD_loadCEntropy=zstdn_ZSTD_loadCEntropy \
	 -DZSTD_loadDEntropy=zstdn_ZSTD_loadDEntropy -DZSTD_maxCLevel=zstdn_ZSTD_maxCLevel \
	 -DZSTD_mergeBlockDelimiters=zstdn_ZSTD_mergeBlockDelimiters -DZSTD_minCLevel=zstdn_ZSTD_minCLevel \
	 -DZSTD_nextInputType=zstdn_ZSTD_nextInputType \
	 -DZSTD_nextSrcSizeToDecompress=zstdn_ZSTD_nextSrcSizeToDecompress \
	 -DZSTD_noCompressLiterals=zstdn_ZSTD_noCompressLiterals \
	 -DZSTD_referenceExternalSequences=zstdn_ZSTD_referenceExternalSequences \
	 -DZSTD_resetCStream=zstdn_ZSTD_resetCStream -DZSTD_resetDStream=zstdn_ZSTD_resetDStream \
	 -DZSTD_resetSeqStore=zstdn_ZSTD_resetSeqStore \
	 -DZSTD_reset_compressedBlockState=zstdn_ZSTD_reset_compressedBlockState \
	 -DZSTD_selectBlockCompressor=zstdn_ZSTD_selectBlockCompressor \
	 -DZSTD_selectEncodingType=zstdn_ZSTD_selectEncodingType -DZSTD_seqToCodes=zstdn_ZSTD_seqToCodes \
	 -DZSTD_sizeof_CCtx=zstdn_ZSTD_sizeof_CCtx -DZSTD_sizeof_CDict=zstdn_ZSTD_sizeof_CDict \
	 -DZSTD_sizeof_CStream=zstdn_ZSTD_sizeof_CStream -DZSTD_sizeof_DCtx=zstdn_ZSTD_sizeof_DCtx \
	 -DZSTD_sizeof_DDict=zstdn_ZSTD_sizeof_DDict -DZSTD_sizeof_DStream=zstdn_ZSTD_sizeof_DStream \
	 -DZSTD_toFlushNow=zstdn_ZSTD_toFlushNow -DZSTD_updateTree=zstdn_ZSTD_updateTree \
	 -DZSTD_versionNumber=zstdn_ZSTD_versionNumber -DZSTD_versionString=zstdn_ZSTD_versionString \
	 -DZSTD_writeLastEmptyBlock=zstdn_ZSTD_writeLastEmptyBlock \
	 -DZSTD_writeSkippableFrame=zstdn_ZSTD_writeSkippableFrame \
	 -Dzstd_cctx_workspace_bound=zstdn_zstd_cctx_workspace_bound \
	 -Dzstd_compress_bound=zstdn_zstd_compress_bound -Dzstd_compress_cctx=zstdn_zstd_compress_cctx \
	 -Dzstd_compress_stream=zstdn_zstd_compress_stream \
	 -Dzstd_cstream_workspace_bound=zstdn_zstd_cstream_workspace_bound \
	 -Dzstd_dctx_workspace_bound=zstdn_zstd_dctx_workspace_bound \
	 -Dzstd_decompress_dctx=zstdn_zstd_decompress_dctx \
	 -Dzstd_decompress_stream=zstdn_zstd_decompress_stream \
	 -Dzstd_dstream_workspace_bound=zstdn_zstd_dstream_workspace_bound \
	 -Dzstd_end_stream=zstdn_zstd_end_stream \
	 -Dzstd_find_frame_compressed_size=zstdn_zstd_find_frame_compressed_size \
	 -Dzstd_flush_stream=zstdn_zstd_flush_stream -Dzstd_get_error_code=zstdn_zstd_get_error_code \
	 -Dzstd_get_error_name=zstdn_zstd_get_error_name \
	 -Dzstd_get_frame_header=zstdn_zstd_get_frame_header -Dzstd_get_params=zstdn_zstd_get_params \
	 -Dzstd_init_cctx=zstdn_zstd_init_cctx -Dzstd_init_cstream=zstdn_zstd_init_cstream \
	 -Dzstd_init_dctx=zstdn_zstd_init_dctx -Dzstd_init_dstream=zstdn_zstd_init_dstream \
	 -Dzstd_is_error=zstdn_zstd_is_error -Dzstd_max_clevel=zstdn_zstd_max_clevel \
	 -Dzstd_min_clevel=zstdn_zstd_min_clevel -Dzstd_reset_cstream=zstdn_zstd_reset_cstream \
	 -Dzstd_reset_dstream=zstdn_zstd_reset_dstream

ccflags-y += -Dxxh32=zstdn_xxh32 -Dxxh32_copy_state=zstdn_xxh32_copy_state -Dxxh32_digest=zstdn_xxh32_digest -Dxxh32_reset=zstdn_xxh32_reset -Dxxh32_update=zstdn_xxh32_update -Dxxh64=zstdn_xxh64 -Dxxh64_copy_state=zstdn_xxh64_copy_state -Dxxh64_digest=zstdn_xxh64_digest -Dxxh64_reset=zstdn_xxh64_reset -Dxxh64_update=zstdn_xxh64_update

ccflags-y += -DFSE_NCountWriteBound=zstdn_FSE_NCountWriteBound -DFSE_buildCTable_raw=zstdn_FSE_buildCTable_raw -DFSE_buildCTable_rle=zstdn_FSE_buildCTable_rle -DFSE_buildCTable_wksp=zstdn_FSE_buildCTable_wksp -DFSE_compressBound=zstdn_FSE_compressBound -DFSE_compress_usingCTable=zstdn_FSE_compress_usingCTable -DFSE_normalizeCount=zstdn_FSE_normalizeCount -DFSE_optimalTableLog=zstdn_FSE_optimalTableLog -DFSE_optimalTableLog_internal=zstdn_FSE_optimalTableLog_internal -DFSE_writeNCount=zstdn_FSE_writeNCount

obj-$(CONFIG_ZSTDN_COMPRESS) += zstdn.o
obj-$(CONFIG_ZSTDN_DECOMPRESS) += zstdn.o

zstdn-y := \
	crypto_zstd.o \
	zstd_compress_module.o \
	xxhash.o \
	common/debug.o \
	common/entropy_common.o \
	common/error_private.o \
	common/fse_decompress.o \
	common/zstd_common.o \
	compress/fse_compress.o \
	compress/hist.o \
	compress/huf_compress.o \
	compress/zstd_compress.o \
	compress/zstd_compress_literals.o \
	compress/zstd_compress_sequences.o \
	compress/zstd_compress_superblock.o \
	compress/zstd_double_fast.o \
	compress/zstd_fast.o \
	compress/zstd_lazy.o \
	compress/zstd_ldm.o \
	compress/zstd_opt.o \
	zstd_decompress_module.o \
	decompress/huf_decompress.o \
	decompress/zstd_ddict.o \
	decompress/zstd_decompress.o \
	decompress/zstd_decompress_block.o
