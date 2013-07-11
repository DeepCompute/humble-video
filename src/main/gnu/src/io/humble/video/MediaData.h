/*******************************************************************************
 * Copyright (c) 2013, Art Clarke.  All rights reserved.
 *  
 * This file is part of Humble-Video.
 *
 * Humble-Video is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Humble-Video is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Humble-Video.  If not, see <http://www.gnu.org/licenses/>.
 *******************************************************************************/

#ifndef MEDIADATA_H_
#define MEDIADATA_H_

#include <io/humble/video/HumbleVideo.h>
#include <io/humble/ferry/RefCounted.h>
#include <io/humble/ferry/IBuffer.h>
#include <io/humble/video/Rational.h>
#include <io/humble/video/KeyValueBag.h>

namespace io { namespace humble { namespace video {

/**
 * The parent class of all media objects than can be gotten from or added to a {@link Stream}.
 */
class VS_API_HUMBLEVIDEO MediaData: public io::humble::ferry::RefCounted
{
public:
  /**
   * Get the time stamp of this object in getTimeBase() units.
   * 
   * @return the time stamp
   */
  virtual int64_t getTimeStamp()=0;
  
  /**
   * Set the time stamp for this object in getTimeBase() units.
   * 
   * @param aTimeStamp The time stamp
   */
  virtual void setTimeStamp(int64_t aTimeStamp)=0;
  
  /**
   * Get the time base that time stamps of this object are represented in.
   * 
   * Caller must release the returned value.
   * 
   * @return the time base.
   */
  virtual Rational* getTimeBase()=0;
  
  /**
   * Set the time base that time stamps of this object are represented in.
   * 
   * @param aBase the new time base.  If null an exception is thrown.
   */
  virtual void setTimeBase(Rational *aBase)=0;
  
  /**
   * Get any underlying raw data available for this object.
   * 
   * @return The raw data, or null if not accessible.
   */
  virtual io::humble::ferry::IBuffer* getData()=0;
  
  /**
   * Get the size in bytes of the raw data available for this object.
   * 
   * @return the size in bytes, or -1 if it cannot be computed.
   */
  virtual int32_t getSize()=0;
  
  /**
   * Is this object a key object?  i.e. it can be interpreted without needing any other media objects
   * 
   * @return true if it's a key, false if not
   */
  virtual bool isKey()=0;

  /**
   * Returns whether or not we think this buffer has been filled
   * with data.
   * 
   * 
   * @return Has setComplete() been called and the buffer populated.
   */
  virtual bool isComplete()=0;

  
protected:
  MediaData();
  virtual ~MediaData();
};

/**
 * The parent class for all Raw media data.
 */
class VS_API_HUMBLEVIDEO MediaRawData: public io::humble::video::MediaData
{
public:
  /** Get the presentation time stamp */
  virtual int64_t getPts() { return getCtx()->pts; }
  /** Get any meta-data associated with this media item */
  virtual KeyValueBag* getMetaData();

  /**
   * pts copied from the Packet that was decoded to produce this frame
   * - encoding: unused
   * - decoding: Read by user.
   */
  virtual int64_t getPacketPts() { return getCtx()->pkt_pts; }

  /**
   * dts copied from the Packet that triggered returning this frame
   * - encoding: unused
   * - decoding: Read by user.
   */
  virtual int64_t getPacketDts() { return getCtx()->pkt_dts; }

  /**
   * size of the corresponding packet containing the compressed
   * frame.
   * It is set to a negative value if unknown.
   * - encoding: unused
   * - decoding: set by libavcodec, read by user.
   */
  virtual int32_t getPacketSize() { return getCtx()->pkt_size; };

  /**
   * duration of the corresponding packet, expressed in
   * ContainerStream.getTimeBase() units, 0 if unknown.
   * - encoding: unused
   * - decoding: Read by user.
   */
  virtual int64_t getPacketDuration() { return getCtx()->pkt_duration; }

  /**
    * frame timestamp estimated using various heuristics, in stream time base
    * - encoding: unused
    * - decoding: set by libavcodec, read by user.
    */
   virtual int64_t getBestEffortTimeStamp() { return getCtx()->best_effort_timestamp; }

   /**
    * Total size in bytes of the decoded media.
    *
    * @return number of bytes of decoded media
    */
   virtual int32_t getSize()=0;

protected:
    MediaRawData() {}
    virtual ~MediaRawData() {}
    virtual AVFrame *getCtx()=0;
};
/**
 * The parent class for all Encoded media data.
 */
class VS_API_HUMBLEVIDEO MediaEncodedData: public io::humble::video::MediaData
{
  protected:
    MediaEncodedData() {}
    virtual ~MediaEncodedData() {}
};
}}}

#endif /* MEDIADATA_H_ */