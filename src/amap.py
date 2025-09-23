import requests

class AMapAPI:
    def __init__(self, api_key):
        self.api_key = api_key
        self.base_url = "https://restapi.amap.com/v3"

    def get_poi_info(self, keywords, city):
        url = f"{self.base_url}/place/text"
        params = {
            "key": self.api_key,
            "keywords": keywords,
            "city": city,
            "output": "json"
        }
        response = requests.get(url, params=params)
        return response.json()

    def get_navigation_route(self, origin, destination, mode="driving"):
        url = f"{self.base_url}/direction/{mode}"
        params = {
            "key": self.api_key,
            "origin": origin,
            "destination": destination,
            "output": "json"
        }
        response = requests.get(url, params=params)
        return response.json()

if __name__ == "__main__":
    api_key = "b67b154d09cca974917f6f1ccac8d0df"
    amap = AMapAPI(api_key)

    # Example usage
    poi_info = amap.get_poi_info("restaurant", "Beijing")
    print(poi_info)

    navigation_route = amap.get_navigation_route("116.481028,39.989643", "116.434446,39.90816")
    print(navigation_route)