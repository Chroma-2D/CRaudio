### What
CRaudio is a low level audio library used by Chroma Framework.  

This library might work with other projects I suppose, but there's no way in hell I'm going to help you with that.

### Why
Because it's easier to prepare data for OpenAL in C than it is in C#.

### How to build from scratch
Clone and build static libogg and libvorbis libraries. Make sure to make these `-fPIC` and `-Wl,--whole-archive`
for static linkage on Linux or you're going to have issues with unresolved symbols.

Repositories in this organization are already modified for such a build.  
I probably butchered them but I don't care - they work. And if they stop working I'll fix them I guess.
