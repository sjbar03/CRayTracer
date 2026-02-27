import sys
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

fname = sys.argv[1] if len(sys.argv) > 1 else 'vectors.csv'
print(f'Reading {fname}...')

# Read normal from the first two non-empty lines, then samples after a blank line
with open(fname, 'r') as fh:
    lines = [l.strip() for l in fh.readlines()]

# find first non-empty line index
idx = 0
while idx < len(lines) and lines[idx] == '':
    idx += 1
# header (normal_x,normal_y,normal_z)
idx += 1
normal_vals = [float(v) for v in lines[idx].split(',')]
idx += 1

# skip possible blank line(s) until the samples header
while idx < len(lines) and lines[idx] == '':
    idx += 1
# now samples header (x,y,z)
idx += 1

from io import StringIO
sample_text = '\n'.join(lines[idx:])
if sample_text.strip() == '':
    data = np.empty((0,3))
else:
    data = np.loadtxt(StringIO(sample_text), delimiter=',')
    if data.ndim == 1:
        data = data.reshape(1, -1)

x = data[:,0] if data.size else np.array([])
y = data[:,1] if data.size else np.array([])
z = data[:,2] if data.size else np.array([])

nx, ny, nz = normal_vals

fig = plt.figure(figsize=(9,9))
ax = fig.add_subplot(111, projection='3d')
ax.scatter(x, y, z, s=1, alpha=0.6)

# plot normal vector as an arrow from origin
ax.quiver(0,0,0, nx, ny, nz, length=1.0, color='r', linewidth=2)

# plot the plane orthogonal to the normal (centered at origin)
# construct two orthonormal basis vectors in the plane
n = np.array([nx, ny, nz], dtype=float)
norm = np.linalg.norm(n)
if norm == 0:
    print('Normal is zero; skipping plane plot')
else:
    n = n / norm
    # pick arbitrary vector not parallel to n
    arbitrary = np.array([1.0, 0.0, 0.0])
    if abs(np.dot(arbitrary, n)) > 0.9:
        arbitrary = np.array([0.0, 1.0, 0.0])
    u = np.cross(n, arbitrary)
    u = u / np.linalg.norm(u)
    v = np.cross(n, u)

    # plane grid
    s = 0.6
    grid = 20
    ru = np.linspace(-s, s, grid)
    rv = np.linspace(-s, s, grid)
    UU, VV = np.meshgrid(ru, rv)
    PX = np.outer(UU.flatten(), u[0]) + np.outer(VV.flatten(), v[0])
    PY = np.outer(UU.flatten(), u[1]) + np.outer(VV.flatten(), v[1])
    PZ = np.outer(UU.flatten(), u[2]) + np.outer(VV.flatten(), v[2])
    PX = PX.reshape(UU.shape)
    PY = PY.reshape(UU.shape)
    PZ = PZ.reshape(UU.shape)
    ax.plot_surface(PX, PY, PZ, color='cyan', alpha=0.2, linewidth=0)

ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')
ax.set_title(f'RandomSurf Vectors ({len(x)} points)')
plt.show()
