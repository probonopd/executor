#if !defined(_REGION_H_)
#define _REGION_H_

/* region.h
 */

#define RGN_SIZE_MASK (0x7FFF)
#define RGN_SPECIAL_FLAG (0x8000)

#define RGNP_SIZE(rgnp) ((rgnp)->rgnSize & RGN_SIZE_MASK)
#define RGNP_SPECIAL_P(rgnp) \
    (!!((rgnp)->rgnSize & RGN_SPECIAL_FLAG))
#define RGNP_SMALL_P(rgnp) \
    (RGNP_SIZE(rgnp) == RGN_SMALL_SIZE)

#define RGNP_SET_SIZE_AND_SPECIAL(rgnp, size, special_p)     \
    ((void)((rgnp)->rgnSize = ((size & RGN_SIZE_MASK) \
                               | ((special_p)                \
                                      ? RGN_SPECIAL_FLAG   \
                                      : 0))))

#define RGNP_SET_SMALL(rgnp) \
    RGNP_SET_SIZE_AND_SPECIAL(rgnp, RGN_SMALL_SIZE, false)

#define RGNP_SET_SIZE(rgnp, size)                               \
    do {                                                        \
        RgnPtr __rgnp = (rgnp);                                 \
        int16_t __orig_size_x;                                  \
                                                                \
        __orig_size_x = __rgnp->rgnSize;                        \
        __rngp->rgnSize = ((__orig_size_x & RGN_SPECIAL_FLAG) \
                           | (size & RGN_SIZE_MASK));    \
    } while(false)
#define RGNP_SET_SPECIAL(rgnp, special_p)                 \
    do {                                                  \
        RgnPtr __rgnp = (rgnp);                           \
                                                          \
        if(special_p)                                     \
            __rgnp->rgnSize |= RGN_SPECIAL_FLAG;        \
        else                                              \
            __rgnp->rgnSize &= ~RGN_SPECIAL_FLAG;       \
    } while(false)

#define RGNP_BBOX(rgnp) ((rgnp)->rgnBBox)

#define RGNP_DATA(rgnp) ((INTEGER *)(rgnp) + 5)



#define RGN_SIZE(rgn) (RGNP_SIZE(*(rgn)))
#define RGN_SPECIAL_P(rgn) (RGNP_SPECIAL_P(*(rgn)))
#define RGN_SMALL_P(rgn) (RGNP_SMALL_P(*(rgn)))

#define RGN_SET_SMALL(rgn) \
    RGNP_SET_SMALL(*(rgn))

#define RGN_SET_SIZE_AND_SPECIAL(rgn, size, special_p) \
    (RGNP_SET_SIZE_AND_SPECIAL(*rgn, size, special_p))
#define RGN_SET_SIZE(rgn, size) \
    RGNP_SET_SIZE(*rgn, size)
#define RGN_SET_SPECIAL(rgn, special_p) \
    RGNP_SET_SPECIAL(*rgn, special_p)

#define RGN_BBOX(rgn) (RGNP_BBOX(*(rgn)))



#define RGN_DATA(rgn) (RGNP_DATA(*(rgn)))

#endif /* !_REGION_H_ */
