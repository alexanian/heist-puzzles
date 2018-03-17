import requests
import time
from queue import Queue
from threading import Thread
from os import path, remove

from Config.Cameras import IPS

padding = 9

q = Queue()
del_q = Queue()
num_threads = len(IPS)
threads = []
del_threads = []


def convert_ip_to_safe(ip):
    return ip.split('/')[-1].replace(':', '-')


def pad(n):
    file_name_padding = str(n)
    while len(file_name_padding) < padding:
        file_name_padding = '0' + file_name_padding

    return file_name_padding


def worker():
    """
    Worker thread
    """
    while True:
        task = q.get()

        if task is None:
            time.sleep(5)
            continue

        ip, num = task

        url = ip + '/camera.jpg'

        try:
            r = requests.get(url)

            safe_ip = convert_ip_to_safe(ip)
            file_name = '{}-{}.jpg'.format(safe_ip, pad(num))
            file_path = path.join('.', 'images', file_name)

            with open(file_path, 'wb') as file:
                file.write(r.content)

        except:
            print('Error with camera: ' + ip)

        finally:
            if num >= 30:
                del_q.put((safe_ip, num - 30))

            time.sleep(0.25)
            q.put((ip, num + 1))
            q.task_done()


def deleter():
    while True:
        task = del_q.get()

        if task is None:
            time.sleep(5)
            continue

        ip, num = task

        file_name = '{}-{}.jpg'.format(ip, pad(num))
        file_path = path.join('.', 'images', file_name)
        remove(file_path)

        time.sleep(0.25)
        del_q.task_done()


if __name__ == '__main__':
    for i in range(num_threads):
        t = Thread(target=worker, daemon=True)
        t.start()
        threads.append(t)

    for i in range(num_threads):
        t = Thread(target=deleter, daemon=True)
        t.start()
        del_threads.append(t)

    for host in IPS:
        q.put((host, 0))

    q.join()
