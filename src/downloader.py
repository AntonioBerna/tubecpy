from pytube import YouTube
import sys
import os

class VideoDownloader:
    def __init__(self, url):
        self.url = url

    def download(self) -> bool:
        try:
            video = YouTube(self.url)
            stream = video.streams.get_highest_resolution()
            if not os.path.exists("videos"): os.makedirs("videos")
            stream.download(output_path="videos")
            return 0
        except Exception:
            return 1

if __name__ == "__main__":
    downloader = VideoDownloader(sys.argv[1])
    sys.exit(downloader.download())
