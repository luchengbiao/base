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
#include "knmusicbackendbass.h"
#include <vector>
#include <io.h>
#include <stdio.h>


KNMusicBackendBass::KNMusicBackendBass(std::wstring dllPath) :
    KNMusicStandardBackend(),
    m_pluginList(std::list<HPLUGIN>())
{
	b_init_player_ = false;
	s_dll_path_ = dllPath;
	InitPlayer();
}

void KNMusicBackendBass::InitPlayer()
{
	//Initial a empty thread flags.
	DWORD threadFlag = 0;
	//Initial the bass library.
	b_init_player_ = initialBass(threadFlag);
	if (!b_init_player_){
		return;
	}

	// load addio dll
	loadPlugin(s_dll_path_);
	//Load plugins from application resource dir and global plugin dir.
	//Initial the main and preview thread.
	setMainThread(generateThread(threadFlag));
	setPreviewThread(generateThread(threadFlag));
}

KNMusicBackendBass::~KNMusicBackendBass()
{
    //Free all the plugin to recover the memory.
	for each (HPLUGIN val in m_pluginList)
	{
        BASS_PluginFree(val);
    }
    //Close the bass.
    BASS_Free();
}

int KNMusicBackendBass::volume() const
{

	return b_init_player_ ? BASS_GetConfig(BASS_CONFIG_GVOL_STREAM) : 0;
}

int KNMusicBackendBass::minimalVolume() const
{
    return 0;
}

int KNMusicBackendBass::maximumVolume() const
{
    return 10000;
}

int64_t KNMusicBackendBass::previewDuration() const
{
	return b_init_player_ ? KNMusicStandardBackend::previewDuration() : 0;
}

int64_t KNMusicBackendBass::previewPosition() const
{

	return b_init_player_ ? KNMusicStandardBackend::previewPosition() : 0;
}

bool KNMusicBackendBass::previewLoadMusic(const std::wstring &filePath,
                                          const int64_t &start,
										  const int64_t &duration)
{
    //Do original event.
	return b_init_player_ ? KNMusicStandardBackend::previewLoadMusic(filePath, start, duration) : false;
}

int KNMusicBackendBass::previewState() const
{
    //Do orignal event.
	return b_init_player_ ? KNMusicStandardBackend::previewState() : 0;
}

void KNMusicBackendBass::previewPlay()
{
    //Do orignal event.
	if (b_init_player_)
	{
		KNMusicStandardBackend::previewPlay();
	}
}

void KNMusicBackendBass::previewPause()
{
    //Do orignal event.
	if (b_init_player_)
	{
		KNMusicStandardBackend::previewPause();
	}
}

void KNMusicBackendBass::previewStop()
{
    //Do orignal event.
	if (b_init_player_)
	{
		KNMusicStandardBackend::previewStop();
	}
}

void KNMusicBackendBass::previewReset()
{
    //Do orignal event.
	if (b_init_player_)
	{
		KNMusicStandardBackend::previewReset();
	}
}

void KNMusicBackendBass::setGlobalVolume(const int &volume)
{
    //Change the global volume size of the bass.
	if (b_init_player_)
	{
		BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, volume);
	}
}

int KNMusicBackendBass::volumeLevel() const
{
    return 1000;
}

double KNMusicBackendBass::smartVolumeScale() const
{
    return 0.125;
}

inline bool KNMusicBackendBass::initialBass(DWORD &channelFlags)
{
    //Detect operating system version and enable option for using WASAPI.

    //Check the bass library version first.
    if(HIWORD(BASS_GetVersion()) > BASSVERSION)
    {
        //Failed to load a higher version bass library.
        return false;
    }
    //Enabled float digital signal processing.
    //DON'T MOVE THIS, this should config before bass init.
    BASS_SetConfig(BASS_CONFIG_FLOATDSP, TRUE);

    //Set a default initial sample rate.
    int initialSampleRate=44100;

    //Normal bass initialize.
    //Prepare the bass initial flag.
    DWORD initFlag=0;
    //Initial bass library.
    if(!BASS_Init(-1, initialSampleRate, initFlag, NULL, NULL))
    {
        //Failed to initial the library bass.
        return false;
    }

    //Clear the channel flags.
    channelFlags=0;
    //Check float dsp supporting.
    DWORD fdpsCheck=BASS_StreamCreate(initialSampleRate,
                                      2, BASS_SAMPLE_FLOAT, NULL, 0);
    //If support the float dsp,
    if(fdpsCheck)
    {
        //Free the check channel, recover the memory.
        BASS_StreamFree(fdpsCheck);
        //Set fdps support flag.
        channelFlags |= BASS_SAMPLE_FLOAT;
    }
    //Load complete.
    return true;
}

void KNMusicBackendBass::GetCurDirFiles(std::wstring path, std::vector<std::wstring>& files)
{
	//文件句柄 
	long  hFile = 0;
	//文件信息 
	struct _wfinddata_t fileinfo;
	std::wstring p = L"";
	if ((hFile = _wfindfirst(p.assign(path).append(L"\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				/*if (wcscmp(fileinfo.name, L".") != 0 && wcscmp(fileinfo.name, L"..") != 0)
				{
				std::wstring file = path;
				file.append(L"\\").append(fileinfo.name);
				files.push_back(file);
				}*/

			}
			else
			{
				std::wstring file = path;
				file.append(fileinfo.name);
				files.push_back(file);
			}
		} while (_wfindnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

inline void KNMusicBackendBass::loadPlugin(const std::wstring &dirPath)
{
	std::wstring dylibSuffix = L".dll";
	//Get all the file from the plugin dir.
	std::vector<std::wstring> vec_file;
	GetCurDirFiles(dirPath, vec_file);
	//Check all the file.
	for each (std::wstring file in vec_file)
	{
		//If the item is file, and the suffix is dynamic linked library, then
		//we have to tried to load the plugin.
		if (file.find(L"bass_") != std::wstring::npos
			&& file.find_last_of(dylibSuffix) != std::wstring::npos)
		{
			//Load the plugin.
			HPLUGIN plugin = BASS_PluginLoad(file.data(), 0);
			//Add the plugin to the plugin list.
			m_pluginList.push_back(plugin);
		}
	}
}

KNMusicBackendBassThread *KNMusicBackendBass::generateThread(
        const DWORD &channelFlags)
{
    //Generate a thread.
    KNMusicBackendBassThread *thread=new KNMusicBackendBassThread();
    //Set the channel create flag to the thread.
    thread->setCreateFlags(channelFlags);
    //Give back the thread.
    return thread;
}
