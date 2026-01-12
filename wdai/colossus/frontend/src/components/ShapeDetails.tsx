import { useParams } from "react-router-dom";
import type { IElement } from "../pages/Main";

function ShapeDetails() {
  const params = useParams();

  const id = parseInt(params.id!);

  const data = localStorage.getItem("elements");
  const elements = data ? JSON.parse(data) : "";

  const element = elements.filter((e: IElement) => e.id === id)[0];

  return (
    <div>
      <p>ID: {element.id}</p>
      {element.data != "" && <p>Data: {element.data}</p>}
      <div className={element.element}></div>
    </div>
  );
}

export default ShapeDetails;
