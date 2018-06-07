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

#ifndef KNMUSICSTANDARDBACKENDTHREAD_H
#define KNMUSICSTANDARDBACKENDTHREAD_H
#include <stdint.h>
#include <string>
/*!
 * \brief The KNMusicStandardBackendThread class provides a interface for
 * standard music backend thread. All the thread should play their own music
 * without borthering other threads.\n
 * This thread abstract interface is used for standard backend.
 */
class KNMusicStandardBackendThread 
{
public:
    /*!
     * \brief Construct a KNMusicStandardBackendThread object with given parent.
     * \param parent The parent standard backend object.
     */
    KNMusicStandardBackendThread(){}

    /*!
     * \brief Load a music file.
     * \param filePath The file path.
     * \return If the thread load the music successful, then return true.
     */
    virtual bool loadFile(const std::wstring &filePath)=0;

    /*!
     * \brief Clear the playing state and the playing file information. The
     * thread after reset will be free.
     */
    virtual void reset()=0;

    /*!
     * \brief Stop playing. This will reset the playing state.
     */
    virtual void stop()=0;

    /*!
     * \brief Start to play the thread.
     */
    virtual void play()=0;

    /*!
     * \brief Pause a playing thread. If the thread is stopped this function
     * should do nothing.
     */
    virtual void pause()=0;

    /*!
     * \brief Get the volume of the current thread.
     * \return The volume size of the thread.
     */
    virtual int volume()=0;

    /*!
     * \brief Get the duration of the playing file. If there's no file playing
     * or the file loaded failed, this should return -1.
     * \return The duration of the playing file in msecond.
     */
    virtual int64_t duration()=0;

    /*!
     * \brief Get the current position of a playing file. If there's no file
     * playing or the file laoded failed, this should return -1. If the current
     * thread is loaded and it's stopped, this should be 0.
     * \return The position of the playing file in msecond.
     */
	virtual int64_t position() = 0;

    /*!
     * \brief Get the playing state of the thread.
     * \return The playing state.
     */
    virtual int state() const=0;

    /*!
     * \brief Set the specific section of the music, treat the section as a
     * whole music file. The start position should be the whether -1 or a number
     * greater than 0 and lesser than duration(). The start position add the
     * duration should not greater than the duration().\n
     * If the duration is not valid, we will play to the end of the file.
     * \param start The start position in msecond.
     * \param duration The duration of the section in msecond.
     */
	virtual void setPlaySection(const int64_t &start = -1,
		const int64_t &duration = -1) = 0;


	/*!
	* \brief Set the volume size of this thread.
	* \param volume The volume size of the thread.
	*/
	virtual void setVolume(const int &volume) = 0;

	/*!
	* \brief Change the position of the current thread.
	* \param position The position of the specifc song.
	*/
	virtual void setPosition(const int64_t &position) = 0;
};

#endif // KNMUSICSTANDARDBACKENDTHREAD_H
