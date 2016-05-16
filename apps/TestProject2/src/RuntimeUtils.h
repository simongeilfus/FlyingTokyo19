/*
 TestProject
 Copyright (c) 2016, Simon Geilfus, All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "runtime_ptr.h"
#include "cinder/Utilities.h"


template<class T>
void test()
{
	T t( ci::vec2(0) );
}

template<class T>
void registerClass( const std::string &name )
{
	auto options = runtime_class<T>::Options().cinder();
	auto interpreter = runtime_class<T>::initialize( name, options );
	
	
	// We need to find the path starting from the location of the app
	auto appRoot = ci::app::getAppPath().parent_path().parent_path().parent_path();
	auto cinderRoot = appRoot.parent_path().parent_path() / "cinder";
	auto blocksFolder = cinderRoot / "blocks";
	
	// Header-only blocks are easily added by just providing the include path
	interpreter->AddIncludePath( ( blocksFolder / "SimplexNoise/include" ).string() );
	interpreter->AddIncludePath( ( blocksFolder / "SpacePartitioning/include" ).string() );
	interpreter->AddIncludePath( ( blocksFolder / "Watchdog/include" ).string() );
	
	// Some blocks requires the main include folder but also the include folder of the library it wraps
	interpreter->AddIncludePath( ( blocksFolder / "Cinder-Cereal/include" ).string() );
	interpreter->AddIncludePath( ( blocksFolder / "Cinder-Cereal/lib/cereal/include" ).string() );
	
	// Some other blocks with .cpp files might be a bit more tricky especially when
	// there's some #defines that need to be processed before the rest like in Cinder-ImGui
	auto imguiFolder = blocksFolder / "Cinder-ImGui";
	interpreter->AddIncludePath( ( imguiFolder / "lib/imgui" ).string() );
	interpreter->AddIncludePath( ( imguiFolder / "include" ).string() );
	interpreter->loadFile( ( imguiFolder / "include/CinderImGui.h" ).string(), false );
	interpreter->enableRawInput();
	interpreter->declare( ci::loadString( ci::loadFile( imguiFolder / "lib/imgui/imgui_draw.cpp" ) ) + "\n" + ci::loadString( ci::loadFile( imguiFolder / "lib/imgui/imgui.cpp" ) ) + "\n" + ci::loadString( ci::loadFile( imguiFolder / "src/CinderImGui.cpp" ) ) );
	interpreter->enableRawInput( false );
}