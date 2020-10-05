import requests, json, time
from threading import Timer
from xlwt import Workbook

def workbook_init(wb, day):
	day.write(0, 0, "TIME")
	day.write(0, 1, "TEMPERATURE (F)")
	day.write(0, 2, "ATMOSPHERIC PRESSURE (hPa)")
	day.write(0, 3, "HUMIDITY (%)")
	day.write(0, 4, "DESCRIPTION")

def get_weather(key, city_name, day, col):
	base_url = "http://api.openweathermap.org/data/2.5/weather?"
	complete_url = base_url + "q=" + city_name + "&appid=" + key

	# send API request
	response = requests.get(complete_url)
	request_data = response.json()

	# check to see if city valid
	if request_data["cod"] != "404":
		weather_data = request_data["main"]

		# parse request data
		current_temperature = (9 / 5) * (weather_data["temp"] - 273.15) + 32
		current_pressure = weather_data["pressure"]
		current_humidity = weather_data["humidity"]
		description_data = request_data["weather"]
		weather_description = description_data[0]["description"]

		# output data
		day.write(col + 1, 0, time.time())
		day.write(col + 1, 1, current_temperature)
		day.write(col + 1, 2, current_pressure)
		day.write(col + 1, 3, current_humidity)
		day.write(col + 1, 4, weather_description)
	else:
		print("City not found")

def main():
	api_key = "93b27e09177a4d6be7c9e6b17a056166"
	city = "Fremont"
	count = 0

	wb = Workbook()
	day = wb.add_sheet("SUNDAY")
	workbook_init(wb, day)

	while(count < 48):
		t = Timer(30 * 60, get_weather, args=(api_key, city, day, count))
		t.start()
		t.join()
		count += 1
	wb.save('weather.xls')
if __name__ == '__main__':
	main()