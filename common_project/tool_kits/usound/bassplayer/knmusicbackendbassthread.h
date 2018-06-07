/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICBACKENDBASSTHREAD_H
#define KNMUSICBACKENDBASSTHREAD_H

#include "bass.h"
#include "knmusicstandardbackendthread.h"
#include <string>
#include <stdint.h>
#include <list>

/*!
 * \brief The KNMusicBackendBassThread class is the standard playing thread for
 * the bass backend. It should be constructed only in the KNMusicBackendBass.
 */
class KNMusicBackendBassThread : public KNMusicStandardBackendThread
{
public:
    /*!
     * \brief Construct a KNMusicBackendBassThread object.
     * \param parent The KNMusicBackendBass object.
     */
    KNMusicBackendBassThread();
    ~KNMusicBackendBassThread();

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::loadFile().
     */
    bool loadFile(const std::wstring &filePath) override;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::reset().
     */
	void reset() override;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::stop().
     */
	void stop() override;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::play().
     */
	void play() override;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::pause().
     */
	void pause() override;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::volume().
     */
	int volume() override;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::duration().
     * \return
     */
	int64_t duration() override;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::position().
     */
	int64_t position() override;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::state().
     */
	int state() const override;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::setPlaySection().
     */
    void setPlaySection(const int64_t &start=-1,
						const int64_t &duration = -1) override;

	/*!
	* \brief Reimplemented from KNMusicStandardBackendThread::setVolume().
	*/
	void setVolume(const int &volume) override;

	/*!
	* \brief Reimplemented from KNMusicStandardBackendThread::setPosition().
	*/
	void setPosition(const int64_t &position) override;

	/*!
	* \brief Set the flags when using at creating a stream.
	* \param channelFlags The new channel flags.
	*/
	void setCreateFlags(const DWORD &channelFlags);
	
private:
    static void CALLBACK threadReachesEnd(HSYNC handle,
                                          DWORD channel,
                                          DWORD data,
                                          void *user);
    inline void finishPlaying();
    inline void resetChannelDuration()
    {
        //Set the duration to the total duration.
        m_duration=m_totalDuration;
        //Set the start position at the very beginning.
        m_startPosition=0;
        //Set the default end position as the whole file.
        m_endPosition=m_duration;
    }

    inline void setPlayingState(const int &state);
    inline void setChannelSyncs()
    {
        m_syncHandlers.push_back(BASS_ChannelSetSync(m_channel,
                                                  BASS_SYNC_END,
                                                  0,
                                                  threadReachesEnd,
                                                  this));
    }

    inline void removeChannelSyncs();
    inline int64_t getChannelPosition()
    {
        //Get the current position
		return (int64_t)
                (BASS_ChannelBytes2Seconds(
                     m_channel,
                     BASS_ChannelGetPosition(m_channel, BASS_POS_BYTE))
                 //Translate the unit from second to msecond, remove the start
                 //position msecond.
                 *1000)-m_startPosition;
    }
    inline double getChannelVolume();
    inline void freeChannel();
    inline bool loadBassThread(const std::wstring &filePath);
	void checkPosition();

    //Channel data.
    std::wstring m_filePath;
    DWORD m_channel;
    DWORD m_channelFlags;

    //Channel information.
    int64_t m_totalDuration,
           m_duration,
           m_startPosition,
           m_endPosition;
	int64_t m_savedPosition;
    double m_volume;
    int m_state;

    //Sync Handlers.
    std::list<HSYNC> m_syncHandlers;
};

#endif // KNMUSICBACKENDBASSTHREAD_H
