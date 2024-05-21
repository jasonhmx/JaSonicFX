import wave
import xml.etree.ElementTree as ET


def read_wav(file_path):
    with wave.open(file_path, "rb") as wf:
        sample_rate = wf.getframerate()
        bit_depth = wf.getsampwidth() * 8
        num_frames = wf.getnframes()
        samples = wf.readframes(num_frames)
        return sample_rate, bit_depth, num_frames, samples


def write_xml(coefficients, sample_rate, bit_depth, num_samples, xml_file):
    root = ET.Element("FilterCoeffs")
    a_k = ET.SubElement(root, "a_k")
    b_k = ET.SubElement(root, "b_k")
    for coeff in coefficients:
        ET.SubElement(b_k, "coeff").text = str(coeff)

    # Add sample rate, bit depth, and number of samples as attributes
    root.set("sample_rate", str(sample_rate))
    root.set("bit_depth", str(bit_depth))
    root.set("num_samples", str(num_samples))

    tree = ET.ElementTree(root)
    tree.write(xml_file, encoding="utf-8", xml_declaration=True)


def main():
    wav_file = "ir.wav"
    xml_file = "coefficients.xml"

    sample_rate, bit_depth, num_samples, samples = read_wav(wav_file)
    coefficients = [
        sample / (2 ** (bit_depth - 1)) for sample in samples
    ]  # Scale samples to floating point representation

    write_xml(coefficients, sample_rate, bit_depth, num_samples, xml_file)
    print("XML file has been created successfully.")


if __name__ == "__main__":
    main()
