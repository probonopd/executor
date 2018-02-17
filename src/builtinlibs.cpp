#include <rsys/builtinlibs.h>
#include <rsys/cfm.h>
#include <rsys/pef.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <cassert>

#include <PowerCore.h>

using namespace Executor;

namespace
{
    std::unordered_map<std::string, std::vector<map_entry_t>> builtins;

    struct PPCCallback
    {
        GUEST_STRUCT;
        GUEST<GUEST<uint32_t>*> codePtr;
        GUEST<uint32_t> rtoc;
        GUEST<uint32_t> sc;
        uint32_t (*implementation)(PowerCore&);
    };

    PPCCallback callbacks[4096];
    int nCallbacks = 0;

}

uint32_t builtinlibs::handleSC(PowerCore& cpu)
{
    PPCCallback *cb = GuestTypeTraits<PPCCallback*>::reg_to_host(cpu.CIA - 8);
    int index = cb - callbacks;
    if(index >= 0 && index < nCallbacks)
    {
        return cb->implementation(cpu);
    }
    else
    {
        std::cerr << "Illegal PPC system call\n";
        std::abort();
    }
}

void builtinlibs::addPPCEntrypoint(const char *library, const char *function, uint32_t (*code)(PowerCore&))
{
    assert(nCallbacks < NELEM(callbacks));
    PPCCallback& callback = callbacks[nCallbacks++];
    callback.sc = CL(0x44000002);
    callback.codePtr = RM(&callback.sc);
    callback.implementation = code;

    builtins[library].push_back({function, &callback});
}

ConnectionID builtinlibs::getBuiltinLib(Str255 libname)
{
    std::string name(libname + 1, libname + 1 + libname[0]);
    auto it = builtins.find(name);
    if(it == builtins.end())
        return nullptr;
    else
    {
        ConnectionID cid = ROMlib_new_connection(1);
        cid->lihp = RM(ROMlib_build_pef_hash(it->second.data(), it->second.size()));
        cid->ref_count = CL(1);
        return cid;
    }
}
