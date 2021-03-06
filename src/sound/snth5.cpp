/* Copyright 1992 by Abacus Research and
 * Development, Inc.  All rights reserved.
 */

#include <base/common.h>
#include <SoundMgr.h>
#include <sound/soundopts.h>

using namespace Executor;

/*
 * It's not really clear how an actual synthesizer is expected to work
 * with the Sound Manager's queues.  In this implementation we start
 * the relavent command playing and see that the ROMLIB_soundcomplete()
 * function is called when we're done.
 */


Boolean Executor::C_snth5(SndChannelPtr chanp, SndCommand *cmdp,
                          ModifierStubPtr mp)
{
#if defined(MACOSX_)
    soundbuffer_t *bufp;
    Boolean done;
    static Boolean beenhere = 0;

    if(!beenhere)
    {
        /* ROMlib_soundreserve(); */
        beenhere = 1;
    }
    done = true;
    switch(cmdp->cmd)
    {
        case initCmd:
            LM(SoundActive) = soundactive5;
            /* TODO */
            break;
        case freeCmd:
            LM(SoundActive) = 0;
            done = false;
            /* TODO */
            break;
        case quietCmd:
            /* TODO */
            break;
        case flushCmd:
            /* TODO */
            break;
        case waitCmd:
            /* TODO */
            break;
        case pauseCmd:
            /* TODO */
            break;
        case resumeCmd:
            /* TODO */
            break;
        case callBackCmd:
#if 0
	printf("CB"); fflush(stdout);
#endif
            chanp->callBack(chanp, cmdp);
            break;
        case syncCmd:
            /* TODO */
            break;
        case availableCmd:
            done = false;
            /* TODO */
            break;
        case bufferCmd:
            bufp = (soundbuffer_t *)cmdp->param2;
#if 0
	printf("offset = %d, nsamples = %d, rate = 0x%x\n", bufp->offset,
						   bufp->nsamples, bufp->rate);
	printf("BU"); fflush(stdout);
#endif
            ROMlib_outbuffer((char *)bufp->buf, bufp->nsamples, bufp->rate,
                             chanp);
            done = false;
            break;

        case requestNextCmd: /* not needed */
        case tickleCmd: /* not implemented */
        case howOftenCmd: /* not implemented */
        case wakeUpCmd: /* not implemented */
        case noteCmd: /* not implemented */
        case restCmd: /* not implemented */
        case freqCmd: /* not implemented */
        case ampCmd: /* not implemented */
        case timbreCmd: /* not implemented */
        case waveTableCmd: /* not implemented */
        case phaseCmd: /* not implemented */
        case soundCmd: /* not implemented */
        case rateCmd: /* not implemented */
        case emptyCmd: /* does nothing */
        case nullCmd: /* does nothing */
        case midiDataCmd: /* not implemented */
        default:
#if 1
            printf("unexpected sound command %d\n", (LONGINT)cmdp->cmd);
#endif
            break;
    }
    if(done)
        ROMlib_callcompletion(chanp);
#endif
    return false;
}
