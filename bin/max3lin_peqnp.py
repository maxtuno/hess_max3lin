"""
///////////////////////////////////////////////////////////////////////////////
//        Copyright (c) 2012-2020 Oscar Riveros. all rights reserved.        //
//                        oscar.riveros@peqnp.science                        //
//                                                                           //
//   without any restriction, Oscar Riveros reserved rights, patents and     //
//  commercialization of this knowledge or derived directly from this work.  //
///////////////////////////////////////////////////////////////////////////////
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

import sys

import peqnp as pn


def load_data(file_name):
    n_, m_, cnf_, eq_ = 0, 0, [], []
    with open(file_name, 'r') as cnf_file:
        lines = cnf_file.readlines()
        for line in filter(lambda x: not x.startswith('c'), lines):
            if line.startswith('p lin'):
                n_, m_ = list(map(int, line[6:].split(' ')))
            else:
                cnf_.append(list(map(int, line.rstrip('\n')[:-2].split(' '))))
                eq_.append(int(cnf_[-1][-1] > 0))
                del cnf_[-1][-1]
    return n_, m_, cnf_, eq_


if __name__ == '__main__':
    n, m, equations, values = load_data(sys.argv[1])
    k = m
    while True:
        pn.engine(bits=m.bit_length())
        x = pn.integer(bits=n)
        assert sum(sum(x[[abs(lit) - 1]](lit < 0, lit > 0) for lit in cls) % 2 ^ v for cls, v in zip(equations, values)) < k
        if pn.satisfy(turbo=True):
            k = sum(sum(lit > 0 if x.binary[abs(lit) - 1] else lit < 0 for lit in cls) % 2 ^ v for cls, v in zip(equations, values))
            print(k)
        else:
            break