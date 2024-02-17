#!/usr/bin/python3

import subprocess
import pytest

class TestBlink:

    @classmethod
    def setup_class(self):
        self.probe_driver = "fx2lafw"
        self.probe_cmd = "sigrok-cli"
        sigrok_cmd = "{} --scan".format(self.probe_cmd)
        output = subprocess.run(sigrok_cmd, shell=True, check=True, capture_output=True, text=True)
        raw_data = output.stdout
        assert self.probe_driver in raw_data

    @classmethod
    def teardown_class(self):
        pass

    def test_blink(self):
        sampling_rate = "20k"
        nb_of_samples = "100k"
        channels = "D0"
        protocol = "pwm"
        sigrok_cmd = "{} -d {} -c samplerate={} --samples {} -C {} -P {}".format(self.probe_cmd, self.probe_driver, sampling_rate, nb_of_samples, channels, protocol)
        output = subprocess.run(sigrok_cmd, shell=True, check=True, capture_output=True, text=True)
        raw_data = output.stdout
        data = raw_data.split("\n")
        counter = 0
        for item in data:
            tmp = item.split(" ")
            if tmp[0] == "":
                break
            if counter % 2 == 0:
                # Duty cycle info
                tmp = tmp[1].split("%")
                duty_cycle = float(tmp[0])
                assert duty_cycle == pytest.approx(50.0, abs=1)
            else:
                # Time info
                time = float(tmp[1])
                assert time == pytest.approx(1.0, abs=0.2)
            counter += 1
