// main
const input = document.querySelector("input");
const preview = document.querySelector(".preview");
const overlay = document.querySelector(".overlay");
const process_button = document.querySelector("#send_images");
const loading = document.querySelector("#loading");

let images;
let processed_images;
initialize_elements();

// adds event listeners to relevant elements
function initialize_elements() {
  // whenever inout element changes, we want to update image preview
  input.addEventListener("change", update_preview);
}

function format_file_size(number) {
  if (number < 1024) {
    return `${number} bytes`;
  } else if (number >= 1024 && number < 1048576) {
    return `${(number / 1024).toFixed(1)} KB`;
  } else if (number >= 1048576) {
    return `${(number / 1048576).toFixed(1)} MB`;
  }
}

function toggle_overlay() {
  overlay.classList.toggle("hidden");
}

function compare_files(ev) {
  toggle_overlay();
  const id = ev.target.parentNode.parentNode.id;
  for (const file of processed_images) {
    if (file.name === id) {
      // we first remove what was in overlay element before
      for (const c of overlay.children) {
        if (c.id !== "hide_overlay") {
          overlay.removeChild(c);
        }
      }

      // image element which shows image overlay
      const image = document.createElement("img");
      image.src = "data:image/png;base64," + file.overlay;

      overlay.appendChild(image);
      return;
    }
  }
}

async function compare_ideal(ev) {
  // show overlay
  toggle_overlay();
  // we first remove what was in overlay element before
  for (const c of overlay.children) {
    if (c.id !== "hide_overlay") {
      overlay.removeChild(c);
    }
  }

  // get id of image and make comparison image
  const id = ev.target.parentNode.parentNode.id;
  const comp = await eel.compare_ideal(id)();

  // create image element and set src
  const image = document.createElement("img");
  image.src = "data:image/png;base64," + comp;
  overlay.appendChild(image);
}

function filter_images(ev) {
  selected = ev.target.selectedOptions[0].label;
  list_items = document.querySelectorAll("ol")[0].children;
  for (let li of list_items) {
    if (li.className != selected && selected != "None") {
      li.style.display = "none";
    } else {
      li.style.display = "flex";
    }
  }
}

function update_preview() {
  // set images to input
  images = input.files;

  // we first remove what was in preview element before
  while (preview.firstChild) {
    preview.removeChild(preview.firstChild);
  }

  // if there are no images, return error text
  if (images.length === 0) {
    const p = document.createElement("p");
    p.className = "error";
    p.textContent = "No images currently selected for processing";
    preview.appendChild(p);
    preview.setAttribute("style", "");
    process_button.disabled = true;
    return;
  }

  // change styles where necessary
  process_button.disabled = false;
  preview.setAttribute("style", "border: 2px solid black");

  // add list element
  const list = document.createElement("ol");
  preview.appendChild(list);

  // loop through images
  for (const file of images) {
    // list item which holds all info
    const listItem = document.createElement("li");

    // paragraph element which shows details of file
    const p = document.createElement("p");
    p.textContent = `File name: ${file.name}`;

    // image element which chose image preview
    const image = document.createElement("img");
    image.src = URL.createObjectURL(file);

    listItem.appendChild(image);
    listItem.appendChild(p);
    list.appendChild(listItem);
  }
}

function update_processed(images) {
  // set processed images to parameter
  processed_images = images;

  // we first remove what was in processed element before
  while (preview.firstChild) {
    preview.removeChild(preview.firstChild);
  }

  // add filter element
  let options = new Set(); // create a Set so that options are unique
  const filter_div = document.createElement("div");
  filter_div.id = "filter";

  const filter_label = document.createElement("label");
  filter_label.textContent = "Select a K Means label:";
  filter_label.setAttribute("for", "options");

  const filter = document.createElement("select");
  filter.onchange = filter_images;
  filter.id = "options";

  filter_div.appendChild(filter_label);
  filter_div.appendChild(filter);
  preview.appendChild(filter_div);

  // add list element
  const list = document.createElement("ol");
  preview.appendChild(list);

  // loop through images
  for (const file of images) {
    // add kmeans label to set of all labels
    if (file.label) options.add(file.label);

    // list item which holds all info
    const listItem = document.createElement("li");
    listItem.id = file.name;
    listItem.className = file.label;

    // paragraph element which shows details of file
    const p = document.createElement("p");
    p.textContent = `File name: ${file.name}`;
    const p2 = document.createElement("p");
    p2.textContent = `Label: ${file.label}`;

    // button element which allows user to compare original image
    const comp = document.createElement("button");
    comp.textContent = "Compare to original";
    comp.onclick = compare_files;
    // button element which allows user to compare original image
    const comp_ideal = document.createElement("button");
    comp_ideal.textContent = "Compare to ideal";
    comp_ideal.disabled = !file.label;
    comp_ideal.onclick = compare_ideal;

    // div item for p and comp
    const div = document.createElement("div");
    div.className = "processed_card";
    div.appendChild(p);
    div.appendChild(p2);
    div.appendChild(comp);
    div.appendChild(comp_ideal);

    // image element which chose image preview
    const image = document.createElement("img");
    image.src = "data:image/png;base64," + file.edited;

    listItem.appendChild(image);
    listItem.appendChild(div);
    list.appendChild(listItem);
  }

  // add options for labels
  options = Array.from(options).sort();
  options.unshift("None");
  options.forEach((option) => {
    let opt = document.createElement("option");
    opt.textContent = option;
    filter.append(opt);
  });

  // hide loading text
  loading.classList.toggle("hidden");
}

function send_images() {
  // show loading text
  loading.classList.toggle("hidden");
  // send files to python
  const curFiles = input.files;
  let fileNames = [];
  for (const f of curFiles) {
    fileNames.push(f.name);
  }
  eel.process_files(fileNames)(update_processed);
}
