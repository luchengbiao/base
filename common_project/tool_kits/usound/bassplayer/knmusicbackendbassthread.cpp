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
#include "knmusicbackendbassthread.h"

enum KNMusicPlayingState
{
	Stopped,
	Playing,
	Paused
};

KNMusicBackendBassThread::KNMusicBackendBassThread() :
    KNMusicStandardBackendThread(),
    m_filePath(L""),
    m_channel(0),
    m_channelFlags(0),
    m_totalDuration(-1),
    m_duration(-1),
    m_startPosition(-1),
    m_endPosition(-1),
    m_savedPosition(-1),
    m_volume(1.0),
    m_state(Stopped),
    m_syncHandlers(std::list<HSYNC>())
{

}

KNMusicBackendBassThread::~KNMusicBackendBassThread()
{
    //Clear up the channel sync handle.
    removeChannelSyncs();
    //Free the channel.
    freeChannel();
}

bool KNMusicBackendBassThread::loadFile(const std::wstring &filePath)
{
    //Stop the current thread first.
    stop();
    //Check is the file the current file.
    if(filePath==m_filePath)
    {
        //Reset the current state.
        resetChannelDuration();
        return true;
    }
    //Clear up all the previous data.
    reset();
    //Load the file path.
    if(!loadBassThread(filePath))
    {
        //Load the file failed.
        return false;
    }
    //When loading the file complete, load the channel info to the thread.
    //Get the duration of the whole file.
    m_totalDuration = BASS_ChannelBytes2Seconds(m_channel, BASS_ChannelGetLength(m_channel, BASS_POS_BYTE)) * 1000.0;
    //Reset the thread information.
    resetChannelDuration();
    //Load complete.
    return true;
}

void KNMusicBackendBassThread::reset()
{
    //Clear up the channel sync handle.
    removeChannelSyncs();
    //Check if the channel is not null.
    freeChannel();
    //Reset the channel data.
    m_filePath.clear();
    //Reset the total duration.
    m_totalDuration=-1;
    //Reset the stream status data.
    resetChannelDuration();
    //Check the current state is stopped or not.
    setPlayingState(Stopped);
}

void KNMusicBackendBassThread::stop()
{
    //Check:
    // 1. The state is already stopped.
    // 2. The channel is null.
    if(m_state==Stopped || (!m_channel))
    {
        return;
    }
    //Stop the channel.
    BASS_ChannelStop(m_channel);

    //Update the state.
    setPlayingState(Stopped);
}

void KNMusicBackendBassThread::play()
{
    //Check:
    // 1. The state is already playing.
    // 2. The channel is null.
    if(m_state==Playing || (!m_channel))
    {
        return;
    }
    //Check the playing state before.
    if(m_state==Stopped)
    {
        //Check out the duration of the song.
        if(m_duration==0)
        {
            //Finished playing, to on action finished.
            finishPlaying();
            //Mission complete.
            return;
        }

        //Set the volume to the last volume, because of the reset, the
        //volume is back to 1.0.
        BASS_ChannelSetAttribute(m_channel, BASS_ATTRIB_VOL, m_volume);
    }


   //Play the thread.
   BASS_ChannelPlay(m_channel, FALSE);
   //Update the state.
   setPlayingState(Playing);
}

void KNMusicBackendBassThread::pause()
{
    //Check:
    // 1. The state is already paused.
    // 2. The channel is null.
    if(m_state==Paused || (!m_channel))
    {
        return;
    }

    //Pause the thread first.
    BASS_ChannelPause(m_channel);

    //Reset the state.
    setPlayingState(Paused);
}

int KNMusicBackendBassThread::volume()
{
    //Scale the float number.
    return (int)(getChannelVolume()*100.0);
}

int64_t KNMusicBackendBassThread::duration()
{
    return m_duration;
}

int64_t KNMusicBackendBassThread::position()
{
    return getChannelPosition();
}

int KNMusicBackendBassThread::state() const
{
    return m_state;
}

void KNMusicBackendBassThread::setPlaySection(const int64_t &start,
												const int64_t &duration)
{
    //Check the validation of the start position.
    // 1. Start is not null(-1).
    // 2. Start is lesser than duration.
    if(start!=-1 && start<m_duration)
    {
        //Save the start position.
        m_startPosition=start;
        //Check the validation of the duration.
        if(duration==-1 || start+duration>m_duration)
        {
            //We will treat the invalid duration when the start is valid to
            //play to the end of the file.
            m_duration-=m_startPosition;
        }
        else
        {
            //Or else, save the new duration.
            m_duration=duration;
        }
        //Update the end position.
        m_endPosition=m_startPosition+m_duration;
    }
}


void KNMusicBackendBassThread::setVolume(const int &volume)
{
	//Check the channel is null.
	if (!m_channel)
	{
		return;
	}
	//The reason why no WASAPI operation here is because that WASAPI will never
	//comes here for setting the preview thread volume.
	//Set the volume to channel.
	BASS_ChannelSetAttribute(m_channel, BASS_ATTRIB_VOL,
		((float)volume) / 100.0);
	//Save the latest volume size.
	m_volume = getChannelVolume();
}


void KNMusicBackendBassThread::setPosition(const int64_t &position)
{
	//Check the channel is null.
	if (!m_channel)
	{
		return;
	}
	//If the state is stopped and the position is not 0, then changed the state
	//to pause.
	if (m_state == Stopped && position != 0)
	{
		//Change to paused.
		setPlayingState(Paused);
	}
	//Change the position, the unit of the position should be translate into
	//second.
	bool flag = BASS_ChannelSetPosition(m_channel, 
		BASS_ChannelSeconds2Bytes(m_channel, (double)(m_startPosition + position) / 1000.0),
		BASS_POS_BYTE);
	//Check the position.
	checkPosition();
}


void KNMusicBackendBassThread::setCreateFlags(const DWORD &channelFlags)
{
	m_channelFlags = channelFlags;
}

void KNMusicBackendBassThread::checkPosition()
{
	//Get the current position.
	int64_t currentPosition = getChannelPosition();

	//Check the position is longer than the duration.
	/*
	* Q: Why we still need to do this?
	* A: When cue is playing, it may not stopped at the end of the file.
	*    The callback is only used to solve the position won't reach the end
	*    bug, the track duration stopped will still process here.
	*/
	if (currentPosition >= m_duration)
	{
		//Finished the playing.
		printf("void KNMusicBackendBassThread::checkPosition() : %lld, %lld \n", currentPosition, m_duration);
		finishPlaying();
	}
}

void KNMusicBackendBassThread::threadReachesEnd(HSYNC handle,
                                                DWORD channel,
                                                DWORD data,
                                                void *user)
{
	if (user)
	{
		static_cast<KNMusicBackendBassThread *>(user)->finishPlaying();
	}
}

inline void KNMusicBackendBassThread::finishPlaying()
{
    //Stop playing.
    stop();
}

inline void KNMusicBackendBassThread::setPlayingState(const int &state)
{
    //Only process the calling when the state is different.
    if(state!=m_state)
    {
        //Save the new state.
        m_state=state;
    }
}

inline void KNMusicBackendBassThread::removeChannelSyncs()
{
    //Get all the handlers.
    for(auto i=m_syncHandlers.begin(); i!=m_syncHandlers.end(); ++i)
    {
        //Remove all the sync from the list.
        BASS_ChannelRemoveSync(m_channel, *i);
    }
    //Clear the hanlder list.
    m_syncHandlers.clear();
}

double KNMusicBackendBassThread::getChannelVolume()
{
    //Check if the thread is loaded.
    if(!m_channel)
    {
        //For a unloaded volume it will be 0.
        return 0.0;
    }
    //Initial a cache.
    float channelVolume;
    //Get the volume to the cache.
    BASS_ChannelGetAttribute(m_channel, BASS_ATTRIB_VOL, &channelVolume);
    //Give back the channel volume.
    return channelVolume;
}

inline void KNMusicBackendBassThread::freeChannel()
{
    //Check if the channel is not null.
    if(m_channel)
    {
        //Free the streams or the music.
        if(!BASS_StreamFree(m_channel))
        {
            BASS_MusicFree(m_channel);
        }
        //Reset the channel.
        m_channel=0;
    }
}

inline bool KNMusicBackendBassThread::loadBassThread(const std::wstring &filePath)
{
    //Clear the file path.
    m_filePath.clear();
    //Try to load the file.
    //Create the file using the stream.
    m_channel=BASS_StreamCreateFile(FALSE,
									filePath.c_str(),
                                    0,
                                    0,
                                    m_channelFlags);
    //Check if the stream create successful.
    if(!m_channel)
    {
        //Set the MOD loading flag.
        DWORD modLoadFlag=BASS_MUSIC_RAMPS;
#ifdef Q_OS_WIN64
        if(m_wasapiEnabled)
        {
            //When using WASAPI, it will only use for decoding.
            modLoadFlag |= BASS_MUSIC_PRESCAN;
        }
#endif
        //Create the file using the fixed music load.
        m_channel=BASS_MusicLoad(FALSE,
								filePath.c_str(),
                                 0,
                                 0,
                                 modLoadFlag | m_channelFlags,
                                 1);
        //Check if the music create successful.
        if(!m_channel)
        {
            //Bass is failed to load the music file.
            return false;
        }
    }
    //Save the new file path.
    m_filePath=filePath;
    //Set the sync handler.
    setChannelSyncs();
    //Load success.
    return true;
}