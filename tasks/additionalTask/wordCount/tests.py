import unittest
from subprocess import Popen, PIPE

TEST_PROGRAM = './additionalTask'


class TestClassTask(unittest.TestCase):

    def callPopen(self):
        with Popen(TEST_PROGRAM, stdout=PIPE, stderr=PIPE, shell=True) as process:
            process.wait()
            assert process.returncode == 0
            return process.stdout.read()

    def test_simple_insert(self):
        expected_lines = [
            b'0:the',
            b'1:a,friends',
            b'2:one,the',
            b'3:a,the',
            b'5:the',
            b'10:the,they',
            b'13:gathering,progressed,the',
            b'14:and,buzzed,more',
            b'15:a',
            b'16:group,the',
            b'17:the',
            b'18:a',
            b'20:the',
            b'21:as,each,farewell,friends',
            b'22:a',
            b'23:the',
            b'24:shared,the',
            b'25:the',
        ]
        process_out = self.callPopen()
        print(process_out)
        print(bytearray(process_out))

        lines = process_out.split(b'\n') 
        print(lines)
        # Check stdout
        for expected_line, line in zip(expected_lines, lines):
            print("expected line {0} to be {1} or {2}".format(expected_line, line, line.replace(b',\b ', b'')))
            self.assertTrue(
                line == expected_line or line.replace(b',\b ', b'') == expected_line or
                all([word in expected_line[expected_line.find(b':') + 1:].split(b',') for word in line[expected_line.find(b':') + 1:].split(b',')]) or
                all([word in expected_line[expected_line.find(b':') + 1:].split(b',') for word in line.replace(b',\b ', b'')[expected_line.find(b':') + 1:].split(b',')])
            )


if __name__ == '__main__':
    unittest.main()