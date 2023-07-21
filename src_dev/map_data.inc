// generated on 2023-07-21 11:07:19
    const auto
models_map = std::unordered_map <std::string, std::vector <std::string>>
{
    { "ISTO_IAPWS_R10", { "10.1063/1.2183324" } },
    { "ISTO_IAPWS_R6_INVERSE", { "10.1063/1.1461829" } },
    { "ISTO_IAPWS_R6", { "10.1063/1.1461829" } },
    { "ISTO_IAPWS_R7", { "10.1115/1.483186", "10.1115/1.1915392", "10.1115/1.2181598", "10.1115/1.2719267", "10.1115/1.3028630" } },
    { "ISTO_IAPWS_TEST_FUNCTIONS", {  } },

};
    map_t
functions_map
{
    // "isto-iapws-r10.json"
    {
          { "ice Ih", "massic enthalpy", { "pressure", "temperature" }, "ISTO_IAPWS_R10" }
        , { reinterpret_cast <void*> (isto::iapws::r10::massic_enthalpy_pt <uv_t, uv_t>), massic_enthalpy, 2, "massic_enthalpy" }
    },
    {
          { "ice Ih", "massic helmholtz energy", { "pressure", "temperature" }, "ISTO_IAPWS_R10" }
        , { reinterpret_cast <void*> (isto::iapws::r10::massic_helmholtz_energy_pt <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "ice Ih", "massic internal energy", { "pressure", "temperature" }, "ISTO_IAPWS_R10" }
        , { reinterpret_cast <void*> (isto::iapws::r10::massic_internal_energy_pt <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "ice Ih", "massic entropy", { "pressure", "temperature" }, "ISTO_IAPWS_R10" }
        , { reinterpret_cast <void*> (isto::iapws::r10::massic_entropy_pt <uv_t, uv_t>), massic_entropy, 2, "massic_entropy" }
    },
    {
          { "ice Ih", "massic isobaric heat capacity", { "pressure", "temperature" }, "ISTO_IAPWS_R10" }
        , { reinterpret_cast <void*> (isto::iapws::r10::massic_isobaric_heat_capacity_pt <uv_t, uv_t>), massic_heat_capacity, 2, "massic_heat_capacity" }
    },
    {
          { "ice Ih", "density", { "pressure", "temperature" }, "ISTO_IAPWS_R10" }
        , { reinterpret_cast <void*> (isto::iapws::r10::density_pt <uv_t, uv_t>), density, 2, "density" }
    },
    {
          { "ice Ih", "cubic expansion coefficient", { "pressure", "temperature" }, "ISTO_IAPWS_R10" }
        , { reinterpret_cast <void*> (isto::iapws::r10::cubic_expansion_coefficient_pt <uv_t, uv_t>), thermal_expansion, 2, "thermal_expansion" }
    },
    {
          { "ice Ih", "pressure coefficient", { "pressure", "temperature" }, "ISTO_IAPWS_R10" }
        , { reinterpret_cast <void*> (isto::iapws::r10::pressure_coefficient_pt <uv_t, uv_t>), pressure_coefficient, 2, "pressure_coefficient" }
    },
    {
          { "ice Ih", "isothermal compressibility", { "pressure", "temperature" }, "ISTO_IAPWS_R10" }
        , { reinterpret_cast <void*> (isto::iapws::r10::isothermal_compressibility_pt <uv_t, uv_t>), compressibility, 2, "compressibility" }
    },
    {
          { "ice Ih", "isentropic compressibility", { "pressure", "temperature" }, "ISTO_IAPWS_R10" }
        , { reinterpret_cast <void*> (isto::iapws::r10::isentropic_compressibility_pt <uv_t, uv_t>), compressibility, 2, "compressibility" }
    },
    {
          { "ice Ih", "massic enthalpy", { "temperature", "pressure" }, "ISTO_IAPWS_R10" }
        , { reinterpret_cast <void*> (isto::iapws::r10::massic_enthalpy_tp <uv_t, uv_t>), massic_enthalpy, 2, "massic_enthalpy" }
    },
    {
          { "ice Ih", "massic helmholtz energy", { "temperature", "pressure" }, "ISTO_IAPWS_R10" }
        , { reinterpret_cast <void*> (isto::iapws::r10::massic_helmholtz_energy_tp <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "ice Ih", "massic internal energy", { "temperature", "pressure" }, "ISTO_IAPWS_R10" }
        , { reinterpret_cast <void*> (isto::iapws::r10::massic_internal_energy_tp <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "ice Ih", "massic entropy", { "temperature", "pressure" }, "ISTO_IAPWS_R10" }
        , { reinterpret_cast <void*> (isto::iapws::r10::massic_entropy_tp <uv_t, uv_t>), massic_entropy, 2, "massic_entropy" }
    },
    {
          { "ice Ih", "massic isobaric heat capacity", { "temperature", "pressure" }, "ISTO_IAPWS_R10" }
        , { reinterpret_cast <void*> (isto::iapws::r10::massic_isobaric_heat_capacity_tp <uv_t, uv_t>), massic_heat_capacity, 2, "massic_heat_capacity" }
    },
    {
          { "ice Ih", "density", { "temperature", "pressure" }, "ISTO_IAPWS_R10" }
        , { reinterpret_cast <void*> (isto::iapws::r10::density_tp <uv_t, uv_t>), density, 2, "density" }
    },
    {
          { "ice Ih", "cubic expansion coefficient", { "temperature", "pressure" }, "ISTO_IAPWS_R10" }
        , { reinterpret_cast <void*> (isto::iapws::r10::cubic_expansion_coefficient_tp <uv_t, uv_t>), thermal_expansion, 2, "thermal_expansion" }
    },
    {
          { "ice Ih", "pressure coefficient", { "temperature", "pressure" }, "ISTO_IAPWS_R10" }
        , { reinterpret_cast <void*> (isto::iapws::r10::pressure_coefficient_tp <uv_t, uv_t>), pressure_coefficient, 2, "pressure_coefficient" }
    },
    {
          { "ice Ih", "isothermal compressibility", { "temperature", "pressure" }, "ISTO_IAPWS_R10" }
        , { reinterpret_cast <void*> (isto::iapws::r10::isothermal_compressibility_tp <uv_t, uv_t>), compressibility, 2, "compressibility" }
    },
    {
          { "ice Ih", "isentropic compressibility", { "temperature", "pressure" }, "ISTO_IAPWS_R10" }
        , { reinterpret_cast <void*> (isto::iapws::r10::isentropic_compressibility_tp <uv_t, uv_t>), compressibility, 2, "compressibility" }
    },
    // "isto-iapws-r6_inverse.json"
    {
          { "water", "density", { "pressure", "temperature" }, "ISTO_IAPWS_R6_INVERSE" }
        , { reinterpret_cast <void*> (isto::iapws::r6_inverse::density_pt <uv_t, uv_t>), density, 2, "density" }
    },
    {
          { "water", "density", { "temperature", "pressure" }, "ISTO_IAPWS_R6_INVERSE" }
        , { reinterpret_cast <void*> (isto::iapws::r6_inverse::density_tp <uv_t, uv_t>), density, 2, "density" }
    },
    {
          { "water", "massic internal energy", { "pressure", "temperature" }, "ISTO_IAPWS_R6_INVERSE" }
        , { reinterpret_cast <void*> (isto::iapws::r6_inverse::massic_internal_energy_pt <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "water", "massic internal energy", { "temperature", "pressure" }, "ISTO_IAPWS_R6_INVERSE" }
        , { reinterpret_cast <void*> (isto::iapws::r6_inverse::massic_internal_energy_tp <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "water", "massic entropy", { "pressure", "temperature" }, "ISTO_IAPWS_R6_INVERSE" }
        , { reinterpret_cast <void*> (isto::iapws::r6_inverse::massic_entropy_pt <uv_t, uv_t>), massic_entropy, 2, "massic_entropy" }
    },
    {
          { "water", "massic entropy", { "temperature", "pressure" }, "ISTO_IAPWS_R6_INVERSE" }
        , { reinterpret_cast <void*> (isto::iapws::r6_inverse::massic_entropy_tp <uv_t, uv_t>), massic_entropy, 2, "massic_entropy" }
    },
    {
          { "water", "massic enthalpy", { "pressure", "temperature" }, "ISTO_IAPWS_R6_INVERSE" }
        , { reinterpret_cast <void*> (isto::iapws::r6_inverse::massic_enthalpy_pt <uv_t, uv_t>), massic_enthalpy, 2, "massic_enthalpy" }
    },
    {
          { "water", "massic enthalpy", { "temperature", "pressure" }, "ISTO_IAPWS_R6_INVERSE" }
        , { reinterpret_cast <void*> (isto::iapws::r6_inverse::massic_enthalpy_tp <uv_t, uv_t>), massic_enthalpy, 2, "massic_enthalpy" }
    },
    {
          { "water", "massic isochoric heat capacity", { "pressure", "temperature" }, "ISTO_IAPWS_R6_INVERSE" }
        , { reinterpret_cast <void*> (isto::iapws::r6_inverse::massic_isochoric_heat_capacity_pt <uv_t, uv_t>), massic_heat_capacity, 2, "massic_heat_capacity" }
    },
    {
          { "water", "massic isochoric heat capacity", { "temperature", "pressure" }, "ISTO_IAPWS_R6_INVERSE" }
        , { reinterpret_cast <void*> (isto::iapws::r6_inverse::massic_isochoric_heat_capacity_tp <uv_t, uv_t>), massic_heat_capacity, 2, "massic_heat_capacity" }
    },
    {
          { "water", "massic isobaric heat capacity", { "pressure", "temperature" }, "ISTO_IAPWS_R6_INVERSE" }
        , { reinterpret_cast <void*> (isto::iapws::r6_inverse::massic_isobaric_heat_capacity_pt <uv_t, uv_t>), massic_heat_capacity, 2, "massic_heat_capacity" }
    },
    {
          { "water", "massic isobaric heat capacity", { "temperature", "pressure" }, "ISTO_IAPWS_R6_INVERSE" }
        , { reinterpret_cast <void*> (isto::iapws::r6_inverse::massic_isobaric_heat_capacity_tp <uv_t, uv_t>), massic_heat_capacity, 2, "massic_heat_capacity" }
    },
    {
          { "water", "massic gibbs free_energy", { "pressure", "temperature" }, "ISTO_IAPWS_R6_INVERSE" }
        , { reinterpret_cast <void*> (isto::iapws::r6_inverse::massic_gibbs_free_energy_pt <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "water", "massic gibbs free_energy", { "temperature", "pressure" }, "ISTO_IAPWS_R6_INVERSE" }
        , { reinterpret_cast <void*> (isto::iapws::r6_inverse::massic_gibbs_free_energy_tp <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "water", "speed of sound", { "pressure", "temperature" }, "ISTO_IAPWS_R6_INVERSE" }
        , { reinterpret_cast <void*> (isto::iapws::r6_inverse::speed_of_sound_pt <uv_t, uv_t>), velocity, 2, "velocity" }
    },
    {
          { "water", "speed of sound", { "temperature", "pressure" }, "ISTO_IAPWS_R6_INVERSE" }
        , { reinterpret_cast <void*> (isto::iapws::r6_inverse::speed_of_sound_tp <uv_t, uv_t>), velocity, 2, "velocity" }
    },
    // "isto-iapws-r6.json"
    {
          { "water", "pressure", { "density", "temperature" }, "ISTO_IAPWS_R6" }
        , { reinterpret_cast <void*> (isto::iapws::r6::pressure_dt <uv_t, uv_t>), pressure, 2, "pressure" }
    },
    {
          { "water", "pressure", { "temperature", "density" }, "ISTO_IAPWS_R6" }
        , { reinterpret_cast <void*> (isto::iapws::r6::pressure_td <uv_t, uv_t>), pressure, 2, "pressure" }
    },
    {
          { "water", "massic internal energy", { "density", "temperature" }, "ISTO_IAPWS_R6" }
        , { reinterpret_cast <void*> (isto::iapws::r6::massic_internal_energy_dt <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "water", "massic internal energy", { "temperature", "density" }, "ISTO_IAPWS_R6" }
        , { reinterpret_cast <void*> (isto::iapws::r6::massic_internal_energy_td <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "water", "massic entropy", { "density", "temperature" }, "ISTO_IAPWS_R6" }
        , { reinterpret_cast <void*> (isto::iapws::r6::massic_entropy_dt <uv_t, uv_t>), massic_entropy, 2, "massic_entropy" }
    },
    {
          { "water", "massic entropy", { "temperature", "density" }, "ISTO_IAPWS_R6" }
        , { reinterpret_cast <void*> (isto::iapws::r6::massic_entropy_td <uv_t, uv_t>), massic_entropy, 2, "massic_entropy" }
    },
    {
          { "water", "massic enthalpy", { "density", "temperature" }, "ISTO_IAPWS_R6" }
        , { reinterpret_cast <void*> (isto::iapws::r6::massic_enthalpy_dt <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "water", "massic enthalpy", { "temperature", "density" }, "ISTO_IAPWS_R6" }
        , { reinterpret_cast <void*> (isto::iapws::r6::massic_enthalpy_td <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "water", "massic isochoric heat capacity", { "density", "temperature" }, "ISTO_IAPWS_R6" }
        , { reinterpret_cast <void*> (isto::iapws::r6::massic_isochoric_heat_capacity_dt <uv_t, uv_t>), massic_heat_capacity, 2, "massic_heat_capacity" }
    },
    {
          { "water", "massic isochoric heat capacity", { "temperature", "density" }, "ISTO_IAPWS_R6" }
        , { reinterpret_cast <void*> (isto::iapws::r6::massic_isochoric_heat_capacity_td <uv_t, uv_t>), massic_heat_capacity, 2, "massic_heat_capacity" }
    },
    {
          { "water", "massic isobaric heat capacity", { "density", "temperature" }, "ISTO_IAPWS_R6" }
        , { reinterpret_cast <void*> (isto::iapws::r6::massic_isobaric_heat_capacity_dt <uv_t, uv_t>), massic_heat_capacity, 2, "massic_heat_capacity" }
    },
    {
          { "water", "massic isobaric heat capacity", { "temperature", "density" }, "ISTO_IAPWS_R6" }
        , { reinterpret_cast <void*> (isto::iapws::r6::massic_isobaric_heat_capacity_td <uv_t, uv_t>), massic_heat_capacity, 2, "massic_heat_capacity" }
    },
    {
          { "water", "massic gibbs free energy", { "density", "temperature" }, "ISTO_IAPWS_R6" }
        , { reinterpret_cast <void*> (isto::iapws::r6::massic_gibbs_free_energy_dt <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "water", "massic gibbs free energy", { "temperature", "density" }, "ISTO_IAPWS_R6" }
        , { reinterpret_cast <void*> (isto::iapws::r6::massic_gibbs_free_energy_td <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "water", "speed of sound", { "density", "temperature" }, "ISTO_IAPWS_R6" }
        , { reinterpret_cast <void*> (isto::iapws::r6::speed_of_sound_dt <uv_t, uv_t>), velocity, 2, "velocity" }
    },
    {
          { "water", "speed of sound", { "temperature", "density" }, "ISTO_IAPWS_R6" }
        , { reinterpret_cast <void*> (isto::iapws::r6::speed_of_sound_td <uv_t, uv_t>), velocity, 2, "velocity" }
    },
    // "isto-iapws-r7.json"
    {
          { "water", "density", { "massic enthalpy", "massic entropy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::density_hs <uv_t, uv_t>), density, 2, "density" }
    },
    {
          { "water", "density", { "massic enthalpy", "pressure" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::density_hp <uv_t, uv_t>), density, 2, "density" }
    },
    {
          { "water", "density", { "massic entropy", "massic enthalpy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::density_sh <uv_t, uv_t>), density, 2, "density" }
    },
    {
          { "water", "density", { "massic entropy", "pressure" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::density_sp <uv_t, uv_t>), density, 2, "density" }
    },
    {
          { "water", "density", { "pressure", "massic enthalpy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::density_ph <uv_t, uv_t>), density, 2, "density" }
    },
    {
          { "water", "density", { "pressure", "massic entropy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::density_ps <uv_t, uv_t>), density, 2, "density" }
    },
    {
          { "water", "density", { "pressure", "temperature" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::density_pt <uv_t, uv_t>), density, 2, "density" }
    },
    {
          { "water", "density", { "temperature", "pressure" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::density_tp <uv_t, uv_t>), density, 2, "density" }
    },
    {
          { "water", "massic enthalpy", { "massic entropy", "pressure" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_enthalpy_sp <uv_t, uv_t>), massic_enthalpy, 2, "massic_enthalpy" }
    },
    {
          { "water", "massic enthalpy", { "pressure", "massic entropy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_enthalpy_ps <uv_t, uv_t>), massic_enthalpy, 2, "massic_enthalpy" }
    },
    {
          { "water", "massic enthalpy", { "pressure", "temperature" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_enthalpy_pt <uv_t, uv_t>), massic_enthalpy, 2, "massic_enthalpy" }
    },
    {
          { "water", "massic enthalpy", { "temperature", "pressure" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_enthalpy_tp <uv_t, uv_t>), massic_enthalpy, 2, "massic_enthalpy" }
    },
    {
          { "water", "massic entropy", { "massic enthalpy", "pressure" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_entropy_hp <uv_t, uv_t>), massic_entropy, 2, "massic_entropy" }
    },
    {
          { "water", "massic entropy", { "pressure", "massic enthalpy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_entropy_ph <uv_t, uv_t>), massic_entropy, 2, "massic_entropy" }
    },
    {
          { "water", "massic entropy", { "pressure", "temperature" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_entropy_pt <uv_t, uv_t>), massic_entropy, 2, "massic_entropy" }
    },
    {
          { "water", "massic entropy", { "temperature", "pressure" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_entropy_tp <uv_t, uv_t>), massic_entropy, 2, "massic_entropy" }
    },
    {
          { "water", "massic internal energy", { "massic enthalpy", "massic entropy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_internal_energy_hs <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "water", "massic internal energy", { "massic enthalpy", "pressure" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_internal_energy_hp <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "water", "massic internal energy", { "massic entropy", "massic enthalpy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_internal_energy_sh <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "water", "massic internal energy", { "massic entropy", "pressure" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_internal_energy_sp <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "water", "massic internal energy", { "pressure", "massic enthalpy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_internal_energy_ph <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "water", "massic internal energy", { "pressure", "massic entropy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_internal_energy_ps <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "water", "massic internal energy", { "pressure", "temperature" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_internal_energy_pt <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "water", "massic internal energy", { "temperature", "pressure" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_internal_energy_tp <uv_t, uv_t>), massic_energy, 2, "massic_energy" }
    },
    {
          { "water", "massic isobaric heat capacity", { "massic enthalpy", "massic entropy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_isobaric_heat_capacity_hs <uv_t, uv_t>), massic_heat_capacity, 2, "massic_heat_capacity" }
    },
    {
          { "water", "massic isobaric heat capacity", { "massic enthalpy", "pressure" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_isobaric_heat_capacity_hp <uv_t, uv_t>), massic_heat_capacity, 2, "massic_heat_capacity" }
    },
    {
          { "water", "massic isobaric heat capacity", { "massic entropy", "massic enthalpy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_isobaric_heat_capacity_sh <uv_t, uv_t>), massic_heat_capacity, 2, "massic_heat_capacity" }
    },
    {
          { "water", "massic isobaric heat capacity", { "massic entropy", "pressure" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_isobaric_heat_capacity_sp <uv_t, uv_t>), massic_heat_capacity, 2, "massic_heat_capacity" }
    },
    {
          { "water", "massic isobaric heat capacity", { "pressure", "massic enthalpy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_isobaric_heat_capacity_ph <uv_t, uv_t>), massic_heat_capacity, 2, "massic_heat_capacity" }
    },
    {
          { "water", "massic isobaric heat capacity", { "pressure", "massic entropy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_isobaric_heat_capacity_ps <uv_t, uv_t>), massic_heat_capacity, 2, "massic_heat_capacity" }
    },
    {
          { "water", "massic isobaric heat capacity", { "pressure", "temperature" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_isobaric_heat_capacity_pt <uv_t, uv_t>), massic_heat_capacity, 2, "massic_heat_capacity" }
    },
    {
          { "water", "massic isobaric heat capacity", { "temperature", "pressure" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_isobaric_heat_capacity_tp <uv_t, uv_t>), massic_heat_capacity, 2, "massic_heat_capacity" }
    },
    {
          { "water", "massic volume", { "massic enthalpy", "massic entropy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_volume_hs <uv_t, uv_t>), massic_volume, 2, "massic_volume" }
    },
    {
          { "water", "massic volume", { "massic enthalpy", "pressure" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_volume_hp <uv_t, uv_t>), massic_volume, 2, "massic_volume" }
    },
    {
          { "water", "massic volume", { "massic entropy", "massic enthalpy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_volume_sh <uv_t, uv_t>), massic_volume, 2, "massic_volume" }
    },
    {
          { "water", "massic volume", { "massic entropy", "pressure" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_volume_sp <uv_t, uv_t>), massic_volume, 2, "massic_volume" }
    },
    {
          { "water", "massic volume", { "pressure", "massic enthalpy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_volume_ph <uv_t, uv_t>), massic_volume, 2, "massic_volume" }
    },
    {
          { "water", "massic volume", { "pressure", "massic entropy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_volume_ps <uv_t, uv_t>), massic_volume, 2, "massic_volume" }
    },
    {
          { "water", "massic volume", { "pressure", "temperature" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_volume_pt <uv_t, uv_t>), massic_volume, 2, "massic_volume" }
    },
    {
          { "water", "massic volume", { "temperature", "pressure" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::massic_volume_tp <uv_t, uv_t>), massic_volume, 2, "massic_volume" }
    },
    {
          { "water", "pressure", { "massic enthalpy", "massic entropy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::pressure_hs <uv_t, uv_t>), pressure, 2, "pressure" }
    },
    {
          { "water", "pressure", { "massic entropy", "massic enthalpy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::pressure_sh <uv_t, uv_t>), pressure, 2, "pressure" }
    },
    {
          { "water", "speed of sound", { "massic enthalpy", "massic entropy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::speed_of_sound_hs <uv_t, uv_t>), velocity, 2, "velocity" }
    },
    {
          { "water", "speed of sound", { "massic enthalpy", "pressure" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::speed_of_sound_hp <uv_t, uv_t>), velocity, 2, "velocity" }
    },
    {
          { "water", "speed of sound", { "massic entropy", "massic enthalpy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::speed_of_sound_sh <uv_t, uv_t>), velocity, 2, "velocity" }
    },
    {
          { "water", "speed of sound", { "massic entropy", "pressure" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::speed_of_sound_sp <uv_t, uv_t>), velocity, 2, "velocity" }
    },
    {
          { "water", "speed of sound", { "pressure", "massic enthalpy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::speed_of_sound_ph <uv_t, uv_t>), velocity, 2, "velocity" }
    },
    {
          { "water", "speed of sound", { "pressure", "massic entropy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::speed_of_sound_ps <uv_t, uv_t>), velocity, 2, "velocity" }
    },
    {
          { "water", "speed of sound", { "pressure", "temperature" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::speed_of_sound_pt <uv_t, uv_t>), velocity, 2, "velocity" }
    },
    {
          { "water", "speed of sound", { "temperature", "pressure" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::speed_of_sound_tp <uv_t, uv_t>), velocity, 2, "velocity" }
    },
    {
          { "water", "temperature", { "massic enthalpy", "massic entropy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::temperature_hs <uv_t, uv_t>), temperature, 2, "temperature" }
    },
    {
          { "water", "temperature", { "massic enthalpy", "pressure" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::temperature_hp <uv_t, uv_t>), temperature, 2, "temperature" }
    },
    {
          { "water", "temperature", { "massic entropy", "massic enthalpy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::temperature_sh <uv_t, uv_t>), temperature, 2, "temperature" }
    },
    {
          { "water", "temperature", { "massic entropy", "pressure" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::temperature_sp <uv_t, uv_t>), temperature, 2, "temperature" }
    },
    {
          { "water", "temperature", { "pressure", "massic enthalpy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::temperature_ph <uv_t, uv_t>), temperature, 2, "temperature" }
    },
    {
          { "water", "temperature", { "pressure", "massic entropy" }, "ISTO_IAPWS_R7" }
        , { reinterpret_cast <void*> (isto::iapws::r7::temperature_ps <uv_t, uv_t>), temperature, 2, "temperature" }
    },
    // "test_functions.json"
    {
          { "test", "temperature", { "temperature", "pressure", "density" }, "ISTO_IAPWS_TEST_FUNCTIONS" }
        , { reinterpret_cast <void*> (isto::remote_services::property::test_functions::add3 <uv_t, uv_t, uv_t>), temperature, 3, "temperature" }
    },
    {
          { "test", "massic enthalpy", { "temperature", "pressure", "density", "velocity" }, "ISTO_IAPWS_TEST_FUNCTIONS" }
        , { reinterpret_cast <void*> (isto::remote_services::property::test_functions::add4 <uv_t, uv_t, uv_t, uv_t>), massic_energy, 4, "massic_energy" }
    },

};
