#pragma once

#include <mbgl/gl/program.hpp>
#include <mbgl/programs/program_parameters.hpp>

#include <cassert>

namespace mbgl {

template <class Shaders, class Primitive, class Attributes, class Uniforms>
class Program : public gl::Program<Primitive, Attributes, Uniforms> {
public:
    using ParentType = gl::Program<Primitive, Attributes, Uniforms>;

    Program(gl::Context& context, const ProgramParameters& programParameters)
        : ParentType(context, vertexSource(programParameters), fragmentSource(programParameters))
        {}

    static std::string fragmentSource(const ProgramParameters& programParameters) {
        std::string fragment = Shaders::fragmentSource;
        
        fragment.insert(0, "\n#define DEVICE_PIXEL_RATIO "+ std::to_string(programParameters.pixelRatio) + "\n");
        
        if (programParameters.overdraw) {
            assert(fragment.find("#ifdef OVERDRAW_INSPECTOR") != std::string::npos);
            fragment.replace(fragment.find_first_of('\n'), 1, "\n#define OVERDRAW_INSPECTOR\n");
        }
        return fragment;
    }
    
    static std::string vertexSource(const ProgramParameters& programParameters){
        std::string vertex = Shaders::vertexSource;
        vertex.replace(vertex.find_first_of('\n'), 1,"\n#define DEVICE_PIXEL_RATIO "+ std::to_string(programParameters.pixelRatio) + "\n");
        return vertex;
    }
};

} // namespace mbgl
