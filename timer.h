
#pragma once

struct Timer
{
    Uint32 mStartTicks;
    Uint32 mPausedTicks;
    bool mPaused;
    bool mStarted;
    Timer ()
    {
       mStartTicks = 0;
       mPausedTicks = 0;
       mPaused = false;
       mStarted = false;
    }
    void start ()
    {
       mStarted = true;
       mPaused = false;
       //Get the current clock time
       mStartTicks = SDL_GetTicks();
	   mPausedTicks = 0;
    }
    //Gets the timer's time
    Uint32 getTicks()
    {
        //The actual timer time
	    Uint32 time = 0;

        //If the timer is running
        if( mStarted )
        {
           //If the timer is paused
           if( mPaused ) time = mPausedTicks;
           else time = SDL_GetTicks() - mStartTicks;
        }
        return time;
    }
};
