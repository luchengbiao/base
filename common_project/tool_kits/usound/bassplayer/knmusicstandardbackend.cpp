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
#include "knmusicstandardbackendthread.h"
#include "knmusicstandardbackend.h"

KNMusicStandardBackend::KNMusicStandardBackend() :
    KNMusicBackend(),
    m_main(nullptr),
    m_preview(nullptr),
    m_originalVolume(-1),
    m_volumeBeforeMute(-1),
    m_mute(false)
{
}

int64_t KNMusicStandardBackend::duration() const
{
    //Get the main thread duration.
    return threadDuration(m_main);
}

int64_t KNMusicStandardBackend::position() const
{
    //Get the main thread position.
    return threadPosition(m_main);
}

bool KNMusicStandardBackend::loadMusic(const std::wstring &filePath,
                                       const int64_t &start,
                                       const int64_t &duration)
{
    //Load the music to the main thread.
    bool loadMainResult=threadLoadMusic(m_main, filePath, start, duration);
	
    return loadMainResult;
}

int KNMusicStandardBackend::state() const
{
	printf("void KNMusicStandardBackend::state()\n");
    //Get the main thread playing state.
    return threadState(m_main);
}

void KNMusicStandardBackend::play()
{
	printf("void KNMusicStandardBackend::play()\n");
    //Play the main thread.
    threadPlay(m_main);
}

void KNMusicStandardBackend::pause()
{
//	printf("void KNMusicStandardBackend::pause()\n");
    //Pause the main thread.
    threadPause(m_main);
}

void KNMusicStandardBackend::stop()
{
//	printf("void KNMusicStandardBackend::stop()\n");
    //Stop the main thread.
    threadStop(m_main);
}

void KNMusicStandardBackend::reset()
{
//	printf("void KNMusicStandardBackend::reset()\n");
    //Reset the main thread.
    threadReset(m_main);
}

int64_t KNMusicStandardBackend::previewDuration() const
{
    //Get the preview thread duration.
    return threadDuration(m_preview);
}

int64_t KNMusicStandardBackend::previewPosition() const
{
    //Get the preview thread position.
    return threadPosition(m_preview);
}

bool KNMusicStandardBackend::previewLoadMusic(const std::wstring &filePath,
                                              const int64_t &start,
											  const int64_t &duration)
{
    //Load the music to preview thread.
    return threadLoadMusic(m_preview, filePath, start, duration);
}

int KNMusicStandardBackend::previewState() const
{
    //Get the preview thread state.
    return threadState(m_preview);
}

void KNMusicStandardBackend::previewPlay()
{
    //If the origial volume is not -1, means smart volume has been turned on.
    if(m_originalVolume==-1)
    {
        //Turn on the smart volume.
        smartVolumeOn();
    }
    //Play the preview thread.
    threadPlay(m_preview);
}

void KNMusicStandardBackend::previewPause()
{
    //Stop the smart volume.
    smartVolumeOff();
    //Pause the preview thread.
    threadPause(m_preview);
}

void KNMusicStandardBackend::previewStop()
{
    //Stop the smart volume.
    smartVolumeOff();
    //Stop the preview thread.
    threadStop(m_preview);
}

void KNMusicStandardBackend::previewReset()
{
    //Stop the smart volume.
    smartVolumeOff();
    //Reset the thread.
    threadReset(m_preview);
}

bool KNMusicStandardBackend::mute()
{
    //Get the global mute state.
    return m_mute;
}


void KNMusicStandardBackend::setVolume(int volumeSize)
{
	//If we want to change the volume, check the mute state first.
	if (mute())
	{
		//Un-mute the backend.
		setMute(false);
	}
	//Check the volume size.
	if (volumeSize<minimalVolume())
	{
		volumeSize = minimalVolume();
	}
	else if (volumeSize>maximumVolume())
	{
		volumeSize = maximumVolume();
	}
	//Sync the thread volume.
	synchronizeThreadVolume(volumeSize);
}

void KNMusicStandardBackend::setPosition(const int64_t &position)
{
//	printf("void KNMusicStandardBackend::setPosition(const int64_t &position): %lld\n", position);
	threadSetPosition(m_main, position);
}

void KNMusicStandardBackend::setPreviewPosition(const int64_t &position)
{
	threadSetPosition(m_preview, position);
}

void KNMusicStandardBackend::setMainThread(KNMusicStandardBackendThread *thread)
{
    //If there's already a main thread, ignore the later sets.
    if(m_main)
    {
        return;
    }
    //Save the main thread.
    m_main=thread;
}

void KNMusicStandardBackend::setPreviewThread(
        KNMusicStandardBackendThread *thread)
{
    //If there's already a preview thread, ignore the later sets.
    if(m_preview)
    {
        return;
    }
    //Save the preview thread.
    m_preview=thread;
}

inline int64_t KNMusicStandardBackend::threadDuration(
        KNMusicStandardBackendThread *thread) const
{
    //The thread cannot be null.
    return thread?thread->duration():-1;
}

inline int64_t KNMusicStandardBackend::threadPosition(
        KNMusicStandardBackendThread *thread) const
{
    //The thread cannot be null.
    return thread?thread->position():-1;
}

inline int KNMusicStandardBackend::threadState(
        KNMusicStandardBackendThread *thread) const
{
    //The thread cannot be null.
    return thread?thread->state():-1;
}

inline bool KNMusicStandardBackend::threadLoadMusic(
        KNMusicStandardBackendThread *thread,
        const std::wstring &filePath,
        const int64_t &start,
        const int64_t &duration)
{
    //Check thread first, if thread exist, load the music.
    if(thread && thread->loadFile(filePath))
    {
        //Set the section for thread.
        thread->setPlaySection(start, duration);
        //Load the music compelte.
        return true;
    }
    //If there's no thread, load music will always be false.
    return false;
}

inline void KNMusicStandardBackend::threadPlay(
        KNMusicStandardBackendThread *thread)
{
    //The thread cannot be null.
    if(thread)
    {
        //Play the thread.
        thread->play();
    }
}

inline void KNMusicStandardBackend::threadPause(
        KNMusicStandardBackendThread *thread)
{
    //The thread cannot be null.
    if(thread)
    {
        //Pause the thread.
        thread->pause();
    }
}

inline void KNMusicStandardBackend::threadStop(
        KNMusicStandardBackendThread *thread)
{
    //The thread cannot be null.
    if(thread)
    {
        //Stop the thread.
        thread->stop();
    }
}

inline void KNMusicStandardBackend::threadReset(
        KNMusicStandardBackendThread *thread)
{
    //The thread cannot be null.
    if(thread)
    {
        //Reset the thread.
        thread->reset();
    }
}

void KNMusicStandardBackend::threadSetPosition(
        KNMusicStandardBackendThread *thread,
        const int64_t &position)
{
    //The thread cannot be null.
    if(thread)
    {
        //Set the position of the thread.
        thread->setPosition(position);
    }
}

inline void KNMusicStandardBackend::synchronizeThreadVolume(int volume)
{
    //Check whether the preview smart volume is enabled.
    bool smartVolumeEnabled=(m_originalVolume!=-1);
    //If the smart volume is enabled, disabled it temporarily.
    if(smartVolumeEnabled)
    {
        //Turn off the smart volume.
        smartVolumeOff();
    }
    //Change the global volume size.
    setGlobalVolume(volume);
    //If the smart volume is enabled before, enabled it again.
    if(smartVolumeEnabled)
    {
        //Turn on the smart volume.
        smartVolumeOn();
    }
}

inline void KNMusicStandardBackend::smartVolumeOn()
{
    //Backup the original volume.
    m_originalVolume=m_main->volume()==0 ? volume() : m_main->volume();
    //Set the preview as the full volume.
    m_preview->setVolume(m_originalVolume);
    //Set the main volume as the smartVolumeScale of the full volume.
    m_main->setVolume((int)((double)m_originalVolume*smartVolumeScale()));
}

inline void KNMusicStandardBackend::smartVolumeOff()
{
    //If the original volume is -1, means it's not turned on.
    if(m_originalVolume==-1)
    {
        return;
    }
    //Set the main volume back to full volume.
    m_main->setVolume(m_originalVolume);
    //Reset the original volume.
    m_originalVolume=-1;
}

void KNMusicStandardBackend::setMute(bool mute)
{
	//Check the state is the same or not, if is the same, do nothing.
	if (m_mute == mute)
	{
		return;
	}
	//Save the new mute state.
	m_mute = mute;
	//Apply the mute state.
	if (m_mute)
	{
		//Backup the original volume.
		m_volumeBeforeMute = volume();
		//Set all the volume to the minimal thread.
		synchronizeThreadVolume(minimalVolume());
	}
	else
	{
		//Set the volume to the backup volume.
		synchronizeThreadVolume(m_volumeBeforeMute);
		//Clear up the backuped volume.
		m_volumeBeforeMute = -1;
	}
}
