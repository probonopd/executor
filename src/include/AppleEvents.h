#if !defined(_AppleEvents_H_)
#define _AppleEvents_H_

/*
 * Copyright 1996 by Abacus Research and Development, Inc.
 * All rights reserved.
 *

 */

#include <EventMgr.h>
#include <NotifyMgr.h>

#define MODULE_NAME AppleEvents
#include <base/api-module.h>

namespace Executor
{
typedef int32_t AEEventClass;
typedef int32_t AEEventID;
typedef int32_t AEKeyword;
typedef ResType DescType;

typedef struct AEDesc
{
    GUEST_STRUCT;
    GUEST<DescType> descriptorType;
    GUEST<Handle> dataHandle;
} AEDesc;

/* ### hack, delete */
typedef AEDesc descriptor_t;

#define DESC_TYPE(desc) ((desc)->descriptorType)
#define DESC_DATA(desc) ((desc)->dataHandle)




typedef struct AEKeyDesc
{
    GUEST<AEKeyword> descKey;
    GUEST<AEDesc> descContent;
} AEKeyDesc;

typedef AEKeyDesc key_desc_t;

#define KEY_DESC_KEYWORD(keydesc) ((keydesc)->descKey)


#define KEY_DESC_CONTENT(keydesc) ((keydesc)->descContent)

typedef AEDesc AEAddressDesc;
typedef AEDesc AEDescList;
typedef AEDescList AERecord;
typedef AERecord AppleEvent;

typedef int32_t AESendMode;

enum
{
    _kAEReplyMask = (0x3),
};

enum
{
    kAENoReply = (0x1),
    kAEQueueReply = (0x2),
    kAEWaitReply = (0x3),
};

enum
{
    _kAEInteractMask = (0x30),
};

enum
{
    kAENeverInteract = (0x10),
    kAECanInteract = (0x20),
    kAEAlwaysInteract = (0x30),
};

enum
{
    kAECanSwitchLayer = (0x40),
    kAEDontReconnect = (0x80),
};
/* #define kAEWantReceipt	??? */

typedef int16_t AESendPriority;

typedef uint8_t AEEventSource;
typedef uint8_t AEInteractionAllowed;
typedef uint8_t AEArrayType;

enum
{
    kAEInteractWithSelf = 0,
    kAEInteractWithLocal = 1,
    kAEInteractWithAll = 2,
};

enum
{
    kAEUnknownSource = 0,
    kAEDirectCall = 1,
    kAESameProcess = 2,
    kAELocalProcess = 3,
    kAERemoteProcess = 4,
};

enum
{
    kAEDataArray,
    kAEPackedArray,
    kAEHandleArray,
    kAEDescArray,
    kAEKeyDescArray
};

enum
{
    kAutoGenerateReturnID = -1,
    kAnyTransactionID = 0
};

enum
{
    kAENormalPriority = 0,
    kAEHighPriority = 1
};

enum
{
    kAEDefaultTimeout = -1,
    kNoTimeOut = -2
};

typedef union AEArrayData {
    GUEST<int16_t> AEDataArray[1];
    int8_t AEPackedArray[1];
    GUEST<Handle> AEHandleArray[1];
    AEDesc AEDescArray[1];
    AEKeyDesc AEKeyDescArray[1];
} AEArrayData;

typedef AEArrayData *AEArrayDataPointer;

typedef ProcPtr IdleUPP;
typedef ProcPtr EventFilterUPP;

typedef UPP<OSErr(const AppleEvent *evt, AppleEvent *reply, int32_t refcon)> AEEventHandlerUPP;
typedef UPP<OSErr(DescType data_type, Ptr data, Size data_size, DescType to_type, int32_t refcon, AEDesc *desc_out)> AECoercePtrUPP;
typedef UPP<OSErr(AEDesc *desc, DescType to_type, int32_t refcon, AEDesc *desc_out)> AECoerceDescUPP;

/* #### internal */

typedef struct AE_hdlr
{
    GUEST_STRUCT;
    GUEST<void *> fn;
    GUEST<int32_t> refcon;
} AE_hdlr_t;

typedef struct AE_hdlr_selector
{
    GUEST_STRUCT;
    GUEST<int32_t> sel0;
    GUEST<int32_t> sel1;
} AE_hdlr_selector_t;

typedef struct AE_hdlr_table_elt
{
    GUEST_STRUCT;
    GUEST<int32_t> pad_1;
    GUEST<AE_hdlr_selector_t> selector;
    GUEST<AE_hdlr_t> hdlr;
    GUEST<int32_t> pad_2;
} AE_hdlr_table_elt_t;

#define AE_TABLE_ELTS(table) ((*table)->elts)

#define AE_TABLE_N_ELTS(table) ((*table)->n_elts)
#define AE_TABLE_N_ALLOCATED_BYTES(table) \
    ((*table)->n_allocated_bytes)

typedef struct AE_hdlr_table
{
    GUEST_STRUCT;
    GUEST<int32_t> pad_1;
    GUEST<int32_t> n_allocated_bytes;
    GUEST<int32_t> n_elts;
    GUEST<int32_t[10]> pad_2;
    GUEST<AE_hdlr_table_elt_t> elts[0];
} AE_hdlr_table_t;

typedef AE_hdlr_table_t *AE_hdlr_table_ptr;

typedef GUEST<AE_hdlr_table_ptr> *AE_hdlr_table_h;

/* points to a 32byte handle of unknown contents (at least,
     sometimes) */
typedef struct AE_zone_tables
{
    GUEST_STRUCT;
    GUEST<AE_hdlr_table_h> event_hdlr_table;
    GUEST<AE_hdlr_table_h> coercion_hdlr_table;
    GUEST<AE_hdlr_table_h> special_hdlr_table;
    GUEST<char[28]> pad_1;
    GUEST<char[4]> unknown_appl_value;
    GUEST<char[8]> pad_2;
    GUEST<Handle> unknown_sys_handle;
} AE_zone_tables_t;

typedef AE_zone_tables_t *AE_zone_tables_ptr;

typedef GUEST<AE_zone_tables_ptr> *AE_zone_tables_h;

/* offset of `appl_zone_tables' is 340; handle to a `struct tables' */

/* offset of `system_zone_tables' is 380; handle to a `struct tables' */

typedef struct
{
    GUEST_STRUCT;
    GUEST<char[340]> pad_1;
    GUEST<AE_zone_tables_h> appl_zone_tables;
    GUEST<char[36]> pad_2;
    GUEST<AE_zone_tables_h> system_zone_tables;
    GUEST<char[212]> pad_3;
} AE_info_t;

typedef AE_info_t *AE_info_ptr;


DISPATCHER_TRAP(Pack8, 0xA816, D0W);

    // "internal". TODO: verify if they really exist as entry points.
extern OSErr C__AE_hdlr_table_alloc(int32_t, int32_t, int32_t, int8_t,
                                                GUEST<AE_hdlr_table_h> *);
PASCAL_SUBTRAP(_AE_hdlr_table_alloc, 0xA816, 0x092E, Pack8);
extern OSErr C__AE_hdlr_delete(AE_hdlr_table_h, int32_t,
                                           AE_hdlr_selector_t *);
PASCAL_SUBTRAP(_AE_hdlr_delete, 0xA816, 0x0632, Pack8);
extern OSErr C__AE_hdlr_lookup(AE_hdlr_table_h, int32_t,
                                           AE_hdlr_selector_t *,
                                           AE_hdlr_t *);
PASCAL_SUBTRAP(_AE_hdlr_lookup, 0xA816, 0x0833, Pack8);
extern OSErr C__AE_hdlr_install(AE_hdlr_table_h, int32_t,
                                            AE_hdlr_selector_t *,
                                            AE_hdlr_t *);
PASCAL_SUBTRAP(_AE_hdlr_install, 0xA816, 0x0831, Pack8);

BEGIN_EXECUTOR_ONLY
/* private */

extern bool send_application_open_aevt_p;
extern bool application_accepts_open_app_aevt_p;

/*  error codes */

#define AE_RETURN_ERROR(error)                         \
    do {                                               \
        OSErr _error_ = (error);                       \
                                                       \
        if(_error_ != noErr)                           \
            warning_unexpected("error `%d'", _error_); \
        return _error_;                                \
    } while(0)
END_EXECUTOR_ONLY

enum
{
    invalidConnection = (-609),
};

enum
{
    errAECoercionFail = (-1700),
    errAEDescNotFound = (-1701),
    errAEWrongDataType = (-1703),
    errAENotAEDesc = (-1704),
};

enum
{
    errAEEventNotHandled = (-1708),
    errAEUnknownAddressType = (-1716),
};

enum
{
    errAEHandlerNotFound = (-1717),
    errAEIllegalIndex = (-1719),
};

/* types */

enum
{
    typeFSS = "fss "_4,

    typeAEList = "list"_4,
    typeAERecord = "reco"_4,
    typeAppleEvent = "aevt"_4,
    typeProcessSerialNumber = "psn "_4,
    typeNull = "null"_4,
    typeApplSignature = "sign"_4,

    typeType = "type"_4,
    typeWildCard = "****"_4,
    typeAlias = "alis"_4,

    typeBoolean = "bool"_4,
    typeChar = "TEXT"_4,
    typeSInt16 = "shor"_4,
    typeSInt32 = "long"_4,
    typeUInt32 = "magn"_4,
    typeSInt64 = "comp"_4,
    typeIEEE32BitFloatingPoint = "sing"_4,
    typeIEEE64BitFloatingPoint = "doub"_4,
    type128BitFloatingPoint = "ldbl"_4,
    typeDecimalStruct = "decm"_4
};

enum
{
    keyAddressAttr = "addr"_4,
    keyEventClassAttr = "evcl"_4,
    keyEventIDAttr = "evid"_4,
    keyProcessSerialNumber = "psn "_4,

    keyDirectObject = "----"_4,
};

enum
{
    kCoreEventClass = "aevt"_4,
    kAEOpenApplication = "oapp"_4,
    kAEOpenDocuments = "odoc"_4,
    kAEPrintDocuments = "pdoc"_4,
    kAEAnswer = "ansr"_4,
    kAEQuitApplication = "quit"_4,

    keySelectProc = "selh"_4,
};

const LowMemGlobal<AE_info_ptr> AE_info { 0x2B6 }; // AppleEvents AEGizmo (true);

/* prototypes go here */

extern OSErr C_AEGetCoercionHandler(DescType from_type, DescType to_type,
                                                GUEST<AECoerceDescUPP> *hdlr_out, GUEST<int32_t> *refcon_out,
                                                GUEST<Boolean> *from_type_is_desc_p_out, Boolean system_handler_p);
PASCAL_SUBTRAP(AEGetCoercionHandler, 0xA816, 0x0B24, Pack8);

extern OSErr C_AECreateDesc(DescType type,
                                        void* data, Size data_size,
                                        AEDesc *desc_out);
PASCAL_SUBTRAP(AECreateDesc, 0xA816, 0x0825, Pack8);
extern OSErr C_AEDisposeDesc(AEDesc *desc);
PASCAL_SUBTRAP(AEDisposeDesc, 0xA816, 0x0204, Pack8);

extern OSErr C_AECoerceDesc(AEDesc *desc, DescType result_type,
                                        AEDesc *desc_out);
PASCAL_SUBTRAP(AECoerceDesc, 0xA816, 0x0603, Pack8);

extern OSErr C_AEGetParamPtr(AERecord *record, AEKeyword keyword,
                                       DescType desired_type, GUEST<DescType> *type_out,
                                       Ptr data, Size max_size, GUEST<Size> *size_out);
PASCAL_SUBTRAP(AEGetParamPtr, 0xA816, 0x0E11, Pack8);

extern OSErr C_AEGetParamDesc(AERecord *record, AEKeyword keyword,
                                        DescType desired_type, AEDesc *desc_out);
PASCAL_SUBTRAP(AEGetParamDesc, 0xA816, 0x0812, Pack8);

extern OSErr C_AEPutParamPtr(AERecord *record, AEKeyword keyword,
                                       DescType type, Ptr data, Size data_size);
PASCAL_SUBTRAP(AEPutParamPtr, 0xA816, 0x0A0F, Pack8);
extern OSErr C_AEPutParamDesc(AERecord *record, AEKeyword keyword,
                                        AEDesc *desc);
PASCAL_SUBTRAP(AEPutParamDesc, 0xA816, 0x0610, Pack8);

/*
extern OSErr C_AEDeleteParam(AppleEvent *evt, AEKeyword keyword);
PASCAL_SUBTRAP(AEDeleteParam, 0xA816, 0x0413, Pack8);
*/

/* The following does not exist. Maybe it should be AEDeleteParam?
extern OSErr C_AEDeleteAttribute(AppleEvent *evt,
                                             AEKeyword keyword);
PASCAL_SUBTRAP_UNKNOWN(AEDeleteAttribute, 0xA816, Pack8);
*/

extern OSErr C_AESizeOfParam(AERecord *record, AEKeyword keyword,
                                           GUEST<DescType> *type_out, GUEST<Size> *size_out);
PASCAL_SUBTRAP(AESizeOfParam, 0xA816, 0x0829, Pack8);

extern OSErr C_AESetInteractionAllowed(AEInteractionAllowed level);
PASCAL_SUBTRAP(AESetInteractionAllowed, 0xA816, 0x011E, Pack8);

extern OSErr C_AEResetTimer(AppleEvent *evt);
PASCAL_SUBTRAP(AEResetTimer, 0xA816, 0x0219, Pack8);

extern OSErr C_AEGetTheCurrentEvent(AppleEvent *return_evt);
PASCAL_SUBTRAP(AEGetTheCurrentEvent, 0xA816, 0x021A, Pack8);
extern OSErr C_AESetTheCurrentEvent(AppleEvent *evt);
PASCAL_SUBTRAP(AESetTheCurrentEvent, 0xA816, 0x022C, Pack8);
extern OSErr C_AESuspendTheCurrentEvent(AppleEvent *evt);
PASCAL_SUBTRAP(AESuspendTheCurrentEvent, 0xA816, 0x022B, Pack8);

extern OSErr C_AEResumeTheCurrentEvent(AppleEvent *evt, AppleEvent *reply,
                                                   AEEventHandlerUPP dispatcher,
                                                   int32_t refcon);
PASCAL_SUBTRAP(AEResumeTheCurrentEvent, 0xA816, 0x0818, Pack8);

extern OSErr C_AEGetInteractionAllowed(AEInteractionAllowed *return_level);
PASCAL_SUBTRAP(AEGetInteractionAllowed, 0xA816, 0x021D, Pack8);

extern OSErr C_AEDuplicateDesc(AEDesc *src, AEDesc *dst);
PASCAL_SUBTRAP(AEDuplicateDesc, 0xA816, 0x0405, Pack8);

extern OSErr C_AECountItems(AEDescList *list, GUEST<int32_t> *count_out);
PASCAL_SUBTRAP(AECountItems, 0xA816, 0x0407, Pack8);

extern OSErr C_AEDeleteItem(AEDescList *list, int32_t index);
PASCAL_SUBTRAP(AEDeleteItem, 0xA816, 0x040E, Pack8);

extern OSErr C_AEDeleteParam(AERecord *record, AEKeyword keyword);
PASCAL_SUBTRAP(AEDeleteParam, 0xA816, 0x0413, Pack8);

extern OSErr C_AEInstallSpecialHandler(AEKeyword function_class,
                                                   AEEventHandlerUPP hdlr,
                                                   Boolean system_handler_p);
PASCAL_SUBTRAP(AEInstallSpecialHandler, 0xA816, 0x0500, Pack8);

extern OSErr C_AERemoveSpecialHandler(AEKeyword function_class,
                                                  AEEventHandlerUPP hdlr,
                                                  Boolean system_handler_p);
PASCAL_SUBTRAP(AERemoveSpecialHandler, 0xA816, 0x0501, Pack8);
extern OSErr C_AEGetSpecialHandler(AEKeyword function_class,
                                               GUEST<AEEventHandlerUPP> *hdlr_out,
                                               Boolean system_handler_p);
PASCAL_SUBTRAP(AEGetSpecialHandler, 0xA816, 0x052D, Pack8);

extern OSErr C_AESend(AppleEvent *evt, AppleEvent *reply,
                                  AESendMode send_mode, AESendPriority send_priority,
                                  int32_t timeout, IdleUPP idle_proc,
                                  EventFilterUPP filter_proc);
PASCAL_SUBTRAP(AESend, 0xA816, 0x0D17, Pack8);

extern OSErr C_AECoercePtr(DescType data_type, Ptr data, Size data_size,
                                       DescType result_type, AEDesc *desc_out);
PASCAL_SUBTRAP(AECoercePtr, 0xA816, 0x0A02, Pack8);

extern OSErr C_AEGetEventHandler(AEEventClass event_class,
                                             AEEventID event_id,
                                             GUEST<AEEventHandlerUPP> *hdlr, GUEST<int32_t> *refcon,
                                             Boolean system_handler_p);
PASCAL_SUBTRAP(AEGetEventHandler, 0xA816, 0x0921, Pack8);

extern OSErr C_AERemoveEventHandler(AEEventClass event_class,
                                                AEEventID event_id,
                                                AEEventHandlerUPP hdlr,
                                                Boolean system_handler_p);
PASCAL_SUBTRAP(AERemoveEventHandler, 0xA816, 0x0720, Pack8);

extern OSErr C_AEProcessAppleEvent(EventRecord *evt);
PASCAL_SUBTRAP(AEProcessAppleEvent, 0xA816, 0x021B, Pack8);

extern OSErr C_AEPutDesc(AEDescList *list, int32_t index,
                                     AEDesc *desc);
PASCAL_SUBTRAP(AEPutDesc, 0xA816, 0x0609, Pack8);

extern OSErr C_AEPutAttributePtr(AppleEvent *evt, AEKeyword keyword,
                                             DescType type, Ptr data, Size size);
PASCAL_SUBTRAP(AEPutAttributePtr, 0xA816, 0x0A16, Pack8);

extern OSErr C_AEPutAttributeDesc(AppleEvent *evt, AEKeyword keyword,
                                              AEDesc *desc);
PASCAL_SUBTRAP(AEPutAttributeDesc, 0xA816, 0x0627, Pack8);

extern OSErr C_AEGetNthPtr(AEDescList *list, int32_t index,
                                       DescType desired_type, GUEST<AEKeyword> *keyword_out,
                                       GUEST<DescType> *type_out,
                                       Ptr data, int32_t max_size, GUEST<int32_t> *size_out);
PASCAL_SUBTRAP(AEGetNthPtr, 0xA816, 0x100A, Pack8);

extern OSErr C_AEGetAttributePtr(AppleEvent *evt, AEKeyword keyword,
                                             DescType desired_type, GUEST<DescType> *type_out,
                                             Ptr data, Size max_size, GUEST<Size> *size_out);
PASCAL_SUBTRAP(AEGetAttributePtr, 0xA816, 0x0E15, Pack8);

extern OSErr C_AEGetArray(AEDescList *list,
                                      AEArrayType array_type,
                                      AEArrayDataPointer array_ptr, Size max_size,
                                      GUEST<DescType> *return_item_type,
                                      GUEST<Size> *return_item_size,
                                      GUEST<int32_t> *return_item_count);
PASCAL_SUBTRAP(AEGetArray, 0xA816, 0x0D0C, Pack8);

extern OSErr C_AECreateAppleEvent(AEEventClass event_class, AEEventID event_id,
                                              AEAddressDesc *target, int16_t return_id, int32_t transaction_id, AppleEvent *evt);
PASCAL_SUBTRAP(AECreateAppleEvent, 0xA816, 0x0B14, Pack8);

extern OSErr C_AEInstallCoercionHandler(DescType from_type, DescType to_type,
                                                    AECoerceDescUPP hdlr, int32_t refcon, Boolean from_type_is_desc_p, Boolean system_handler_p);
PASCAL_SUBTRAP(AEInstallCoercionHandler, 0xA816, 0x0A22, Pack8);

extern OSErr C_AEInstallEventHandler(AEEventClass event_class, AEEventID event_id,
                                                 AEEventHandlerUPP hdlr, int32_t refcon, Boolean system_handler_p);
PASCAL_SUBTRAP(AEInstallEventHandler, 0xA816, 0x091F, Pack8);

extern OSErr C_AERemoveCoercionHandler(DescType from_type, DescType to_type,
                                                   AECoerceDescUPP hdlr, Boolean system_handler_p);
PASCAL_SUBTRAP(AERemoveCoercionHandler, 0xA816, 0x0723, Pack8);

extern OSErr C_AEPutArray(AEDescList *list, AEArrayType type,
                                      AEArrayDataPointer array_data,
                                      DescType item_type,
                                      Size item_size, int32_t item_count);
PASCAL_SUBTRAP(AEPutArray, 0xA816, 0x0B0D, Pack8);

extern OSErr C_AECreateList(Ptr list_elt_prefix, Size list_elt_prefix_size,
                                        Boolean is_record_p, AEDescList *list_out);
PASCAL_SUBTRAP(AECreateList, 0xA816, 0x0706, Pack8);

extern OSErr C_AEGetAttributeDesc(AppleEvent *evt, AEKeyword keyword,
                                              DescType desired_type, AEDesc *desc_out);
PASCAL_SUBTRAP(AEGetAttributeDesc, 0xA816, 0x0826, Pack8);

extern OSErr C_AESizeOfAttribute(AppleEvent *evt, AEKeyword keyword,
                                             GUEST<DescType> *type_out, GUEST<Size> *size_out);
PASCAL_SUBTRAP(AESizeOfAttribute, 0xA816, 0x0828, Pack8);

extern OSErr C_AEGetNthDesc(AEDescList *list, int32_t index,
                                        DescType desired_type, GUEST<AEKeyword> *keyword_out,
                                        AEDesc *desc_out);
PASCAL_SUBTRAP(AEGetNthDesc, 0xA816, 0x0A0B, Pack8);

extern OSErr C_AESizeOfNthItem(AEDescList *list, int32_t index,
                                           GUEST<DescType> *type_out, GUEST<Size> *size_out);
PASCAL_SUBTRAP(AESizeOfNthItem, 0xA816, 0x082A, Pack8);

extern OSErr C_AEPutPtr(AEDescList *list, int32_t index, DescType type,
                                    Ptr data, Size data_size);
PASCAL_SUBTRAP(AEPutPtr, 0xA816, 0x0A08, Pack8);

extern OSErr C_AEInteractWithUser(int32_t timeout, NMRecPtr nm_req,
                                              IdleUPP idle_proc);
PASCAL_SUBTRAP(AEInteractWithUser, 0xA816, 0x061C, Pack8);

extern OSErr C_AEManagerInfo(GUEST<LONGINT> *resultp);
PASCAL_SUBTRAP(AEManagerInfo, 0xA816, 0x0441, Pack8);

extern OSErr C_AEDisposeToken(AEDesc *theToken);
PASCAL_SUBTRAP(AEDisposeToken, 0xA816, 0x023A, Pack8);
extern OSErr C_AEResolve(AEDesc *objectSpecifier,
                                      INTEGER callbackFlags,
                                      AEDesc *theToken);
PASCAL_SUBTRAP(AEResolve, 0xA816, 0x0536, Pack8);
extern OSErr C_AERemoveObjectAccessor(DescType desiredClass,
                                                  DescType containerType,
                                                  ProcPtr theAccessor,
                                                  Boolean isSysHandler);
PASCAL_SUBTRAP(AERemoveObjectAccessor, 0xA816, 0x0738, Pack8);
extern OSErr C_AEInstallObjectAccessor(DescType desiredClass,
                                                   DescType containerType,
                                                   ProcPtr theAccessor,
                                                   LONGINT refcon,
                                                   Boolean isSysHandler);
PASCAL_SUBTRAP(AEInstallObjectAccessor, 0xA816, 0x0937, Pack8);
extern OSErr C_AEGetObjectAccessor(DescType desiredClass,
                                               DescType containerType,
                                               ProcPtr *theAccessor,
                                               LONGINT *accessorRefcon,
                                               Boolean isSysHandler);
PASCAL_SUBTRAP(AEGetObjectAccessor, 0xA816, 0x0939, Pack8);
extern OSErr C_AECallObjectAccessor(DescType desiredClass,
                                                AEDesc *containerToken,
                                                DescType containerClass,
                                                DescType keyForm,
                                                AEDesc *keyData,
                                                AEDesc *theToken);
PASCAL_SUBTRAP(AECallObjectAccessor, 0xA816, 0x0C3B, Pack8);
extern OSErr C_AESetObjectCallbacks(ProcPtr myCompareProc,
                                                ProcPtr myCountProc,
                                                ProcPtr myDisposeTokenProc,
                                                ProcPtr myGetMarkTokenProc,
                                                ProcPtr myMarkProc,
                                                ProcPtr myAdjustMarksProc,
                                                ProcPtr myGetErrDescProc);
PASCAL_SUBTRAP(AESetObjectCallbacks, 0xA816, 0x0E35, Pack8);

static_assert(sizeof(AEArrayData) == 12);
static_assert(sizeof(AEDesc) == 8);
static_assert(sizeof(AEKeyDesc) == 12);
static_assert(sizeof(AE_hdlr_t) == 8);
static_assert(sizeof(AE_hdlr_selector_t) == 8);
static_assert(sizeof(AE_hdlr_table_elt_t) == 24);
static_assert(sizeof(AE_hdlr_table_t) == 52);
static_assert(sizeof(AE_zone_tables_t) == 56);
static_assert(sizeof(AE_info_t) == 596);
static_assert(sizeof(AEArrayData) == 12);
}

#endif /* ! _AppleEvents_H_ */
