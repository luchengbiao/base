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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICBACKEND_H
#define KNMUSICBACKEND_H
#include <stdint.h>
#include <string>
/*!
 * \brief The KNMusicBackend class provides the music playing ports for
 * KNMusicDetailTooltipBase and KNMusicHeaderPlayerBase. All the function should
 * be implement and realized. The backend should support play two music
 * concurrency, and control them independently. The volume should be controlled
 * independently as well.\n
 * There is a defination called 'main thread' and another one called 'preview
 * thread'. The main thread will be used to play the file in the header player,
 * main player and any other parts. The preview thread will only be used at
 * these parts: Lyrics, Preview Tooltip, Details. Only for previewing the file.
 */
class KNMusicBackend 
{
public:
    /*!
     * \brief Construct a KNMusicBackend class with given parent object.
     * \param parent The parent object.
     */
    KNMusicBackend(){}

    /*!
     * \brief Get the duration of the main thread.
     * \return The duration of the main thread music in msecond. If there's no
     * music playing, return -1.
     */
    virtual int64_t duration() const=0;

    /*!
     * \brief Get the current position of the main thread.
     * \return The current playing position of the main thread music in msecond.
     * If there's no music playing, return -1.
     */
	virtual int64_t position() const = 0;

    /*!
     * \brief Load a music file from local hard disk. All the file will be treat
     * at local. You can ask the main thread only played a part of the music.
     * This is designed for those huge music file with a cue sheet file to
     * separate them.
     * \param filePath The music file path.
     * \param start The start position of the music. If you want to play from
     * the start of the whole file, leave it there to the default -1.
     * \param duration The end position of the music. If you want to play to the
     * end of the whole file, leave it there to the default -1.
     * \return If we can get the file and send it to the music playing backend,
     * it will be true. Or else it wil be false. To judge whether has been load
     * successful, wait for a loadSuccess() or loadFailed() signal;
     */
    virtual bool loadMusic(const std::wstring &filePath,
                           const int64_t &start=-1,
						   const int64_t &duration = -1) = 0;

    /*!
     * \brief Play the main thread.
     */
    virtual void play()=0;

    /*!
     * \brief Pause the main thread.
     */
    virtual void pause()=0;

    /*!
     * \brief Stop the main thread, reset the position to the start position.
     */
    virtual void stop()=0;

    /*!
     * \brief Reset the main thread. The state will be reset to stop, and all
     * memory resource of the main thread will be cleared.
     */
    virtual void reset()=0;

    /*!
     * \brief Get the volume of the main size, range from the minimum volume
     * to maximum volume.
     * \return The volume size.
     */
    virtual int volume() const=0;

    /*!
     * \brief Get the state of the main thread.
     * \return The main thread state.
     */
    virtual int state() const=0;

    /*!
     * \brief Get the duration of the file loaded in preview thread.
     * \return The duration of the preview file. If there's no file in the
     * preview thread, it will be -1.
     */
    virtual int64_t previewDuration() const=0;

    /*!
     * \brief Get the position of the file loaded in the preview thread.
     * \return The current playing position of the file loaded in the preview
     * thread. If there's no file in the preview thread, it will be -1.
     */
	virtual int64_t previewPosition() const = 0;

    /*!
     * \brief Load a music file from local to preview thread.
     * \param filePath The music file path.
     * \param start The start position of the music. If you want to play from
     * the start of the whole file, leave it there to the default -1.
     * \param duration The end position of the music. If you want to play to the
     * end of the whole file, leave it there to the default -1.
     * \return If we can get the file and send it to the preview thread, it will
     * be true. Or else it wil be false. To judge whether has been load
     * successful, wait for a previewLoadSuccess() or previewLoadFailed()
     * signal.
     */
    virtual bool previewLoadMusic(const std::wstring &filePath,
									const int64_t &start = -1,
									const int64_t &duration = -1) = 0;

    /*!
     * \brief Play the preview thread.
     */
    virtual void previewPlay()=0;

    /*!
     * \brief Pause the preview thread.
     */
    virtual void previewPause()=0;

    /*!
     * \brief Stop the preview thread and reset the position of the preview
     * thread to the start position.
     */
    virtual void previewStop()=0;

    /*!
     * \brief Reset the preview thread. The state will be reset to stop, and all
     * memory resource of the preview thread will be cleared.
     */
    virtual void previewReset()=0;

    /*!
     * \brief Get the state of the preview thread.
     * \return The state of the preview thread.
     */
    virtual int previewState() const=0;

    /*!
     * \brief Get the minimal volume size of the backend.
     * \return The minimal volume size of the backend.
     */
    virtual int minimalVolume() const=0;

    /*!
     * \brief Get the maximum volume size of the backend.
     * \return The maximum volume size of the backend.
     */
    virtual int maximumVolume() const=0;

    /*!
     * \brief Get whether the backend is in the mute state.
     * \return If the backend has been muted, it will be true.
     */
    virtual bool mute()=0;

	/*!
	* \brief Set the mute state to specific state.
	* \param mute To mute the backend, set true.
	*/
	virtual void setMute(bool mute) = 0;

	/*!
	* \brief Set the volume to a specific size. The size should be between
	* minimalVolume() and maximumVolume(). Or else it won't change anything.
	* \param volumeSize The prefer volume size.
	*/
	virtual void setVolume(int volumeSize) = 0;

	/*!
	* \brief Set the current position of the main thread. If there's no file
	* loaded in the main thread or the position is not positive and less than
	* the duration, nothing will happened.
	* \param position The new position of the main thread.
	*/
	virtual void setPosition(const int64_t &position) = 0;

	virtual void setPreviewPosition(const int64_t &position) = 0;
};

#endif // KNMUSICBACKEND_H
