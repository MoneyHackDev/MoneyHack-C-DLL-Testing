#ifndef REMOTEDLL_H
#define REMOTEDLL_H

#ifdef REMOTEDLL_EXPORTS
#define REMOTEDLL_API __declspec(dllexport)
#else
#define REMOTEDLL_API __declspec(dllimport)
#endif

// Exported function declaration
extern "C" REMOTEDLL_API void RemoteFunction();

#endif // REMOTEDLL_H
