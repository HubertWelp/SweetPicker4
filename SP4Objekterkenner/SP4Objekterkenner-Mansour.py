#!/usr/bin/env python3
"""
SP4Objekterkenner.py

Ein Ersatz für SP3Objekterkenner.py, der die selbe Schnittstelle und Verhalten beibehält,
aber die YOLO-Architektur für die Objekterkennung anstelle der TensorFlow Object Detection API verwendet.

@author: Sahin Cosgun
@date: April 2025
@editor: Mohamed Aziz Mansour
"""

import os
import sys
import socket
import time
import logging
import numpy as np
import cv2
from ultralytics import YOLO
import supervision as sv

# Setup logging
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s - %(name)s - %(levelname)s - %(message)s",
    datefmt="%Y-%m-%d %H:%M:%S",
)
logger = logging.getLogger("SP4Objekterkenner")

# Class mapping from YOLO model names to original model IDs
CLASS_MAPPING = {
    "maoam": 1,
    "snickers": 2,
    "milkyway": 3,
    "riegel": 4,
}


# Network configuration
UDP_IP_ADDRESS = "127.0.0.1"
UDP_PORT_NO = 5850

# File paths
FILE_PATH = "/home/student/git/SP4/SweetPicker4/SP4Bildanalysator/SP3Bilderkennung/"
IMAGE_PATH = os.path.join(FILE_PATH, "aktuelleSzene.jpg")
IMAGE_PATH_DETECTED = os.path.join(FILE_PATH, "gefundeneObjekte.jpg")
TEXT_PATH_DETECTED = os.path.join(FILE_PATH, "gefundeneObjekte.txt")

# Path to YOLO model
MODEL_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "best_yolo11s.pt")


def load_model():
    """
    Load the YOLO model from disk.

    Returns:
        YOLO: The loaded model object

    Raises:
        SystemExit: If the model cannot be loaded
    """
    try:
        logger.info(f"Loading model from {MODEL_PATH}")
        model = YOLO(MODEL_PATH)
        logger.info("Model loaded successfully")
        return model
    except Exception as e:
        logger.error(f"Failed to load model: {e}")
        sys.exit(1)


def run_detection(model, image_path):
    """
    Run object detection on the given image using the YOLO model.

    Args:
        model (YOLO): The loaded YOLO model
        image_path (str): Path to the input image

    Returns:
        dict: Output dictionary with detection results and annotated image,
              or None if detection fails
    """
    try:
        # Read image
        image = cv2.imread(image_path)
        if image is None:
            logger.error(f"Failed to read image from {image_path}")
            return None

        # Run inference
        results = model(image, conf=0.5, verbose=False)[0]

        # Create detections object for visualization
        detections = sv.Detections.from_ultralytics(results)

        # Set up annotators
        box_annotator = sv.BoxAnnotator()
        label_annotator = sv.LabelAnnotator()

        # Annotate the image
        annotated_image = image.copy()
        annotated_image = box_annotator.annotate(
            scene=annotated_image, detections=detections
        )
        annotated_image = label_annotator.annotate(
            scene=annotated_image, detections=detections
        )

        # Initialize arrays for detection results
        boxes = []
        classes = []
        scores = []

        # Process YOLO results to match the expected output format
        height, width = image.shape[:2]

        for i, (box, cls, score) in enumerate(
            zip(
                results.boxes.xyxy.cpu().numpy(),
                results.boxes.cls.cpu().numpy(),
                results.boxes.conf.cpu().numpy(),
            )
        ):
            # Get class name and map to standardized IDs
            class_name = results.names[int(cls)].lower()
            class_id = CLASS_MAPPING.get(class_name, 0)  # Default to 0 if not found

            # Extract box coordinates
            x1, y1, x2, y2 = box

            # Convert to normalized coordinates in [y1, x1, y2, x2] format
            norm_y1 = float(y1 / height)
            norm_x1 = float(x1 / width)
            norm_y2 = float(y2 / height)
            norm_x2 = float(x2 / width)

            # Add to result lists
            classes.append(class_id)
            scores.append(float(score))
            boxes.append([norm_y1, norm_x1, norm_y2, norm_x2])

        # Emulate old filestructure exactly
        if len(classes) < 100:
            padding_count = 100 - len(classes)
            classes.extend(
                [1] * padding_count
            )  # Using 1 (Maoam) as padding class (like in the original)
            scores.extend([0.0] * padding_count)
            boxes.extend([[0.0, 0.0, 0.0, 0.0]] * padding_count)

        # Package results in the expected format
        output_dict = {
            "image": annotated_image,
            "detection_classes": np.array(classes, dtype=np.int64),
            "detection_scores": np.array(scores, dtype=np.float32),
            "detection_boxes": np.array(boxes, dtype=np.float32),
        }
        print("Mapped class IDs in detection_classes:", classes[:10])

        return output_dict

    except Exception as e:
        logger.error(f"Error during detection: {e}", exc_info=True)
        return None


def save_outputs(output_dict):
    """
    Save detection results to image and text files in the exact format
    expected by other components of the system.

    Args:
        output_dict (dict): Dictionary containing detection results

    Returns:
        bool: True if saving was successful, False otherwise
    """
    if output_dict is None:
        logger.error("No output to save")
        return False

    try:
        # Save annotated image
        cv2.imwrite(IMAGE_PATH_DETECTED, output_dict["image"])
        print(f"Pfad:", IMAGE_PATH_DETECTED)
        # Save detection results to text file
        file = open(TEXT_PATH_DETECTED, "w")
        file.write("detection_classes\n")
        file.write(str(output_dict["detection_classes"]))
        file.write(" \ndetection_scores\n")
        file.write(str(output_dict["detection_scores"]))
        file.write("\ndetection_boxes\n")
        file.write(str(output_dict["detection_boxes"]))
        file.close()
        logger.info(
            f"Detection results saved to {IMAGE_PATH_DETECTED} and {TEXT_PATH_DETECTED}"
        )
        return True

    except Exception as e:
        logger.error(f"Error saving outputs: {e}")
        return False


def main():
    """
    Main function that mimics the behavior of the original SP3Objekterkenner.py script.
    - Creates a UDP socket and waits for messages
    - Processes input images when available
    - Saves detection results in the expected format
    """
    try:
        # Load model
        model = load_model()

        # Create UDP socket
        client_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        client_sock.bind((UDP_IP_ADDRESS, UDP_PORT_NO))
        logger.info(f"Listening on {UDP_IP_ADDRESS}:{UDP_PORT_NO}")

        # Main loop - identical control flow to the original
        while True:
            print("---------------------------------------------------")
            print("  Der SP4Objekterkenner wartet auf eine Anfrage..  ")
            print("---------------------------------------------------")

            # Wait for UDP message
            data, addr = client_sock.recvfrom(1024)

            # Check if this is a termination command
            if len(data) > 10:
                logger.info("Received termination command")
                break

            # Check if the input image exists
            if os.path.isfile(IMAGE_PATH):
                # Run detection
                print("Erkennung gestratet")
                start_time = time.time()
                output_dict = run_detection(model, IMAGE_PATH)

                # Save outputs
                if save_outputs(output_dict):
                    print("Erkennung gestratet")
                    end_time = time.time()
                    dauer = end_time - start_time
                    print(f"Erkennung abgeschlossen in {dauer*1000:.2f} ms")
                    print("---------------------------------------------------")
                    print("Die Objekterkennung wurde erfolgreich durchgefuehrt")
                    print("---------------------------------------------------")
            else:
                # Wait 1 second before checking again
                print("---------------------------------------------------")
                print("    Die Bilddatei konnte nicht gefunden werden!    ")
                print("---------------------------------------------------")
                time.sleep(1)

        print("---------------------------------------------------")
        print("    Das Programm SP4Objekterkenner wurde beendet   ")
        print("---------------------------------------------------")

    except Exception as e:
        logger.error(f"Unexpected error: {e}", exc_info=True)
        sys.exit(1)


if __name__ == "__main__":
    main()
