import bpy
import struct
from mathutils import Matrix, Vector

bl_info = {
    "name": "Import-Export: MDL format",
    "description": "Import-Export MDL files",
    "location": "File > Import-Export",
    "version": (0, 1, 0),
    "blender": (3, 0, 0),
}


def get_axis(axis):
    if axis == 'X':
        return Vector((1, 0, 0))
    elif axis == '-X':
        return Vector((-1, 0, 0))
    elif axis == 'Y':
        return Vector((0, 1, 0))
    elif axis == '-Y':
        return Vector((0, -1, 0))
    elif axis == 'Z':
        return Vector((0, 0, 1))
    elif axis == '-Z':
        return Vector((0, 0, -1))


def get_abs_axis(axis):
    if axis == 'X':
        return Vector((1, 0, 0))
    elif axis == '-X':
        return Vector((1, 0, 0))
    elif axis == 'Y':
        return Vector((0, 1, 0))
    elif axis == '-Y':
        return Vector((0, 1, 0))
    elif axis == 'Z':
        return Vector((0, 0, 1))
    elif axis == '-Z':
        return Vector((0, 0, 1))


def select_face(obj, face):
    obj.select_set(True)
    bpy.ops.object.mode_set(mode='EDIT')
    bpy.ops.mesh.select_mode(type='FACE')
    bpy.ops.mesh.select_all(action='DESELECT')
    bpy.ops.object.mode_set(mode='OBJECT')
    face.select = True
    bpy.ops.object.mode_set(mode='EDIT')


class Data:
    def __init__(self, operator):
        self.data = []
        self.operator = operator
        self.objects_count = 0

        right = operator.right_axis
        forward = operator.forward_axis
        up = operator.up_axis

        rightv = get_axis(right)
        forwardv = get_axis(forward)
        upv = get_axis(up)

        self.inv = 1 if rightv.cross(forwardv) == upv else -1

        self.orient3 = Matrix([rightv, forwardv, upv]).transposed()
        self.orient4 = self.orient3.to_4x4()
        print("Orientation")
        print(self.orient4)

        self.orient3abs = Matrix([get_abs_axis(right), get_abs_axis(forward), get_abs_axis(up)]).transposed()
        self.orient4abs = self.orient3abs.to_4x4()

    def report(self, message):
        self.operator.report({"ERROR"}, message)

    def byte(self, value):
        self.data.append(value)

    def int(self, value):
        self.data.extend(bytearray(struct.pack('i', value)))

    def float(self, value):
        self.data.extend(bytearray(struct.pack('f', value)))

    def string(self, value):
        if value is None:
            self.int(0)
            return

        self.int(len(value))
        self.data.extend(str.encode(value))

    def vec2(self, value):
        self.float(value.x)
        self.float(value.y)

    def vec3(self, value):
        self.float(value.x)
        self.float(value.y)
        self.float(value.z)

    def vec4(self, value):
        self.float(value.x)
        self.float(value.y)
        self.float(value.z)
        self.float(value.w)

    def mat4(self, value):
        for v in value:
            self.vec4(v)

    def arrf(self, values):
        for value in values:
            self.float(value)

    def object(self, object, parent_id, materials, all):
        id = self.objects_count
        self.objects_count += 1
        self.string(object.name)

        self.int(parent_id)

        translation = self.orient4 @ object.location
        rotation = self.orient4 @ Vector((object.rotation_euler.x, object.rotation_euler.y, object.rotation_euler.z))
        scale = self.orient4abs @ object.scale

        translation = Matrix.Translation(translation)
        rotation_x = Matrix.Rotation(rotation.x * self.inv, 4, 'X')
        rotation_y = Matrix.Rotation(rotation.y * self.inv, 4, 'Y')
        rotation_z = Matrix.Rotation(rotation.z * self.inv, 4, 'Z')
        scale_x = Matrix.Scale(scale.x, 4, (1, 0, 0))
        scale_y = Matrix.Scale(scale.y, 4, (0, 1, 0))
        scale_z = Matrix.Scale(scale.z, 4, (0, 0, 1))
        transformation = translation @ rotation_z @ rotation_y @ rotation_x @ scale_z @ scale_y @ scale_x

        self.mat4(transformation)

        if object.type == 'MESH':
            self.byte(1)
            result = self.mesh(object, materials)
            if result is not None:
                return result
        else:
            self.byte(0)

        for child in object.children:
            if child.select_get() == True or all == True:
                result = self.object(child, id, materials, all)
                if result is not None:
                    return result
            else:
                self.report(f'{object.name} child {child.name} is not selected')
                return {'FINISHED'}

        print()

        return None

    def mesh(self, obj, mats):
        mesh = obj.data

        vertices = mesh.vertices
        indices = mesh.loops

        self.string(mesh.name)

        materials = mesh.materials
        if len(materials) == 0:
            self.report(f'{mesh.name} has no material')
            return {'FINISHED'}
        elif len(materials) > 1:
            self.report(f'{mesh.name} has more than 1 material')
            return {'FINISHED'}

        material = mesh.materials[0]
        if not material.name in mats:
            mats[material.name] = material

        mat = list(mats).index(material.name)

        self.int(mat)

        if len(mesh.uv_layers) == 0:
            self.report(f'{mesh.name} has no uv layer')
            return {'FINISHED'}
        elif len(mesh.uv_layers) > 1:
            self.report(f'{mesh.name} has more than one uv layer')
            return {'FINISHED'}

        uvs = mesh.uv_layers[0].data

        if len(uvs) == 0:
            self.report(f'{mesh.name} has no uv')
            return {'FINISHED'}

        polygons = mesh.polygons

        def gen_uv_map():
            uv_map = {}
            for face in polygons:
                for vert_idx, loop_idx in zip(face.vertices, face.loop_indices):
                    if vert_idx in uv_map:
                        if uvs[uv_map[vert_idx]].uv != uvs[loop_idx].uv:
                            return None
                    else:
                        uv_map[vert_idx] = loop_idx

            return uv_map

        uv_map = gen_uv_map()

        self.int(1 if uv_map is None else 0)

        self.int(len(vertices))

        for vertex in mesh.vertices:
            self.vec3(self.orient3 @ vertex.co)
            self.vec3(self.orient3 @ vertex.normal)
            if uv_map is not None:
                self.vec2(uvs[uv_map[vertex.index]].uv)

        self.int(len(polygons))

        r = 1 if self.operator.reverse == True else 0

        for face in polygons:
            loop = face.loop_indices
            if len(loop) < 3:
                self.report(f'{mesh.name} contains faces with less than 3 vertices')
                select_face(obj, face)
                return {'FINISHED'}
            elif len(loop) > 3:
                self.report(f'{mesh.name} contains faces with more than 3 vertices')
                select_face(obj, face)
                return {'FINISHED'}

            self.int(indices[loop[0 + 0]].vertex_index)
            self.int(indices[loop[1 + r]].vertex_index)
            self.int(indices[loop[2 - r]].vertex_index)

        if uv_map is None:
            for face in polygons:
                loop = face.loop_indices

                self.vec2(uvs[loop[0 + 0]].uv)
                self.vec2(uvs[loop[1 + r]].uv)
                self.vec2(uvs[loop[2 - r]].uv)

        return None

    def parse_color(self, node, name):
        if len(node.links) == 0:
            return node.default_value, None
        elif len(node.links) == 1:
            tex_node = node.links[0].from_node
            if tex_node.type != 'TEX_IMAGE':
                self.report(f'{name} color node is not supported')
                return None

            image = tex_node.image
            return [1.0, 1.0, 1.0, 1.0], image.name
        else:
            self.report(f'{name} color is not supported')
            return None

    def parse_roughness(self, node, name):
        if len(node.links) == 0:
            return node.default_value
        else:
            self.report(f'{name} roughness is not supported')
            return None

    def parse_metallic(self, node, name):
        if len(node.links) == 0:
            return node.default_value
        else:
            self.report(f'{name} metallic is not supported')
            return None

    def materials(self, materials):
        self.int(len(materials))
        for name, material in materials.items():
            print(name)
            self.string(name)

            diffuse = material.diffuse_color
            diffuse_texture = None
            specular = material.specular_color
            specular = [specular[0], specular[1], specular[2], 1.0]
            specular_texture = None
            roughness = material.roughness
            metallic = material.metallic

            if material.use_nodes:
                nodes = material.node_tree.nodes

                output = None
                for node in nodes:
                    if node.type == 'OUTPUT_MATERIAL':
                        output = node
                        break

                if output is None:
                    self.report(f'{name} output is not supported')
                    return {'FINISHED'}

                surface = output.inputs['Surface']
                if len(surface.links) != 1:
                    self.report(f'{name} surface input is not supported')
                    return {'FINISHED'}
                bsdf = surface.links[0].from_node
                if bsdf.type == 'BSDF_PRINCIPLED':
                    print("BSDF_PRINCIPLED")
                    diffuse, diffuse_texture = self.parse_color(bsdf.inputs['Base Color'], name)
                    if diffuse is None:
                        return {'FINISHED'}
                    specular, specular_texture = self.parse_color(bsdf.inputs['Specular'], name)
                    if specular is None:
                        return {'FINISHED'}
                    roughness = self.parse_roughness(bsdf.inputs['Roughness'], name)
                    if roughness is None:
                        return {'FINISHED'}
                    metallic = self.parse_roughness(bsdf.inputs['Metallic'], name)
                    if metallic is None:
                        return {'FINISHED'}
                elif bsdf.type == 'BSDF_DIFFUSE':
                    print("BSDF_DIFFUSE")
                    diffuse, diffuse_texture = self.parse_color(bsdf.inputs['Color'], name)
                    if diffuse is None:
                        return {'FINISHED'}
                    specular = [1.0, 1.0, 1.0, 1.0]
                    roughness = self.parse_roughness(bsdf.inputs['Roughness'], name)
                    if roughness is None:
                        return {'FINISHED'}
                    metallic = 0.0
                else:
                    self.report(f'{name} surface input node is not supported')
                    return {'FINISHED'}

            if isinstance(specular, float):
                specular = [specular, specular, specular, specular]
            elif isinstance(specular, list):
                if len(specular) == 3:
                    specular = [specular[0], specular[1], specular[2], 1.0]

            print("Diffuse: ", diffuse[0], diffuse[1], diffuse[2], diffuse[3])
            print("Diffuse texture: ", diffuse_texture)
            print("Specular: ", specular[0], specular[1], specular[2], specular[3])
            print("Specular texture: ", specular_texture)
            print("Roughness: ", roughness)
            print("Metallic: ", metallic)

            self.arrf(diffuse)
            self.string(diffuse_texture)
            self.arrf(specular)
            self.string(specular_texture)
            self.float(roughness)
            self.float(metallic)

        return None

    def save_to_file(self, filepath):
        f = open(filepath, 'wb')
        buffer = bytes(self.data)
        f.write(buffer)
        f.close()


def export_model(operator):
    print("exporting...")

    print(bpy.context.active_object)

    if bpy.context.active_object is not None and bpy.context.active_object.mode == 'EDIT':
        bpy.ops.object.mode_set(mode='OBJECT')

    materials = {}
    data = Data(operator)

    if bpy.context.selected_objects:
        objects = bpy.context.selected_objects
        all = False
    else:
        objects = bpy.context.scene.objects
        all = True

    data.int(len(objects))
    for obj in objects:
        if obj.parent is None or (all == False and obj.parent.select_get() == False):
            result = data.object(obj, -1, materials, all)
            if result is not None:
                return result

    result = data.materials(materials)
    if result is not None:
        return result

    data.save_to_file(operator.filepath)

    print()

    return {'FINISHED'}


from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty
from bpy.types import Operator


class ExportModel(Operator, ExportHelper):
    bl_idname = "export.mdl"
    bl_label = "Export MDL"
    bl_options = {'UNDO', 'PRESET'}

    filename_ext = ".mdl"

    filter_glob: StringProperty(
        default="*.mdl",
        options={'HIDDEN'},
        maxlen=255,
    )

    reset_transforms: BoolProperty(
        name="Reset transforms",
        description="Roots transforms will be in world origin",
        default=True,
    )

    split_edges: BoolProperty(
        name="Split edges",
        description="Split edges",
        default=False,
    )

    write_images: BoolProperty(
        name="Write images",
        description="Write images in file",
        default=False,
    )

    reverse: BoolProperty(
        name="Reverse polygons",
        description="Write mesh with reversed polygons order",
        default=True,
    )

    right_axis: EnumProperty(
        name="Right Axis",
        description="Right axis for export",
        items=(
            ('X', "X", "Right is posititive X Axis"),
            ('Y', "Y", "Right is posititive Y Axis"),
            ('Z', "Z", "Right is posititive Z Axis"),
            ('-X', "-X", "Right is negative X Axis"),
            ('-Y', "-Y", "Right is negative Y Axis"),
            ('-Z', "-Z", "Right is negative Z Axis"),
        ),
        default='X',
    )

    forward_axis: EnumProperty(
        name="Forward Axis",
        description="Forward axis for export",
        items=(
            ('X', "X", "Forward is posititive X Axis"),
            ('Y', "Y", "Forward is posititive Y Axis"),
            ('Z', "Z", "Forward is posititive Z Axis"),
            ('-X', "-X", "Forward is negative X Axis"),
            ('-Y', "-Y", "Forward is negative Y Axis"),
            ('-Z', "-Z", "Forward is negative Z Axis"),
        ),
        default='Z',
    )

    up_axis: EnumProperty(
        name="Up Axis",
        description="Up axis for export",
        items=(
            ('X', "X", "Up is posititive X Axis"),
            ('Y', "Y", "Up is posititive Y Axis"),
            ('Z', "Z", "Up is posititive Z Axis"),
            ('-X', "-X", "Up is negative X Axis"),
            ('-Y', "-Y", "Up is negative Y Axis"),
            ('-Z', "-Z", "Up is negative Z Axis"),
        ),
        default='Y',
    )

    def execute(self, _):
        return export_model(self)

    def draw(self, _):
        layout = self.layout
        layout.use_property_split = True
        layout.use_property_decorate = False

        box = layout.box()
        row = box.row(align=True)
        row.prop(self, "reset_transforms")
        row = box.row(align=True)
        row.prop(self, "split_edges")
        row = box.row(align=True)
        row.prop(self, "write_images")
        row = box.row(align=True)
        row.prop(self, "reverse")

        box = layout.box()
        row = box.row(align=True)
        row.label(text="Global Orientation", icon="ORIENTATION_GLOBAL")

        row = box.row(align=True)
        row.prop(self, "right_axis", expand=True)
        row = box.row(align=True)
        row.prop(self, "forward_axis", expand=True)
        row = box.row(align=True)
        row.prop(self, "up_axis", expand=True)


def menu_func_export(self, _):
    self.layout.operator(ExportModel.bl_idname, text="Model (.mdl)")


def register():
    bpy.utils.register_class(ExportModel)
    bpy.types.TOPBAR_MT_file_export.append(menu_func_export)


def unregister():
    bpy.types.TOPBAR_MT_file_export.remove(menu_func_export)
    bpy.utils.unregister_class(ExportModel)


if __name__ == "__main__":
    register()
