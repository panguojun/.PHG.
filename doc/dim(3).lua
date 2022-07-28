--*** Dimensions translate math to sci ***---
--*** const or dimensions ***--

nm = 1e-9	-- m
cm = 0.01	-- cm
mm = 0.001	-- mm

c = 299792458 --m/s

-- Planck constant
h = 6.62607015 * 1e-34 --J·s

-- Boltzmann constant
ke = 1.380649 * 1e-23 -- J/K

sigma = 5.67*1e-8

-- Water specific heat capacity
water_SHC = 4.2 * 1e3		

calorie = 4.1859			
big_cal = calorie * 1e3		

CocaCola_J_100ML = 171 		

---------------Physics-----------------
-- light --
function hv(gama)
	return h * gama;
end

-- [wave] --
function wavelength_frequency(Meter)
	return 1.0 / Meter
end
function frequency_wavelength(Hz)
	return 1.0 / Hz
end
function NM_HZ(NM)
	return 1.0 / (NM * nm);
end
-- temperature --
function celsius_kelvins(C)
	return C+273.15;
end
function kelvins_celsius(T)
	return T-273.15;
end

-- [black body] --
-- [Planck]
function planck(T,gama)
	prt(gama)
	local A = 2 * h * pow(gama, 3) / pow(c, 2);
	local B0 = h * gama / (ke * T);
	local B = math.exp(h * gama / (ke * T));
	prt(B)
	local ret = A * 1 / (B - 1);
	prt(ret)
	return ret;
end
function planck_T_NM(T,NM)
	return planck(T, 1.0 / (NM * nm))
end
function B_T(T)
	return sigma*pow(T,4);
end
-- [wien]
function peakCM_T(CM)
	return 0.29 / CM;
end
function T_peakCM(T)
	return 0.29 / T;
end

function lightP(I, R)
	return I * (1+R) / c;
end

-- [water] --
function water_DT_J(dCelsius)
	return dCelsius * water_SHC
end

---------------astronomy---------------
--*** const ***--
au = 149597870700 -- m 

ly = 9.4607e12	-- m

arc_second	= math.pi / 180.0 / 3600.0 

min_arc_second = 0.01

Proxima_Centauri_dis = 4.2 * ly

-- distance of per arc_second per year 
pc = (au) / arc_second -- example : pc / Proxima_Centauri_dis

rad_deg = math.pi / 180.0

arc_second_degree = arc_second * 180 / math.pi 

-- the sun
earth_sun = au

sun_d = 1.392 * 1e6 * 1e3

sun_s = 4.0/3.0*math.pi*pow(sun_d/2,3)

sun_degree = sun_d / earth_sun * (180 / math.pi)

-- the moon
earth_moon = 384403.9 * 1e3	-- m

moon_d = 3476.28 * 1e3  -- m

moon_degree = moon_d / earth_moon * (180 / math.pi)

-- apparent magnitude --
-- 1-6 --
appafent_magnitude = pow(10.0,-0.4)

vega_flux = 1.0

function flux_mag(flux)
	return -2.5*math.log(flux/vega_flux,10.0);
end
function mag_flux(mag)
	return pow(appafent_magnitude, mag)*vega_flux;
end

-- color index --
function color_index(b,v)
	return brt_mag(b)-brt_mag(v)
end

function M_m(d)
	return 5-5*log(d,10) -- d in pc
end

-- distance modulus
function distance_modulus(m,M)
	return pow(10, m-M+5)/5;
end

--************* main *************--
function main()
	msgbox(color_index(1.3,1))
	
end

main()