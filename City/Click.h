/**
 * \file Click.h
 *
 * \author Charles Owen
 *
 * Interface class for click history items.
 */
#pragma once

#include <memory>
class CTile;

/**
 * Interface class for click history items.
 */
class CClick
{
public:
    /** Constructor */
    CClick() {}

    /** Destructor */
    virtual ~CClick() {}

    /**
     * Get the tile we clicked on 
     * \returns CTile object pointer
     */
    virtual std::shared_ptr<CTile> GetTile() = 0;

    /**
    * Get the time this click occurred
    * \returns Time as a long long value (seconds since 1970)
    */
    virtual time_t GetTime() = 0;
};

